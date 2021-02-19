#include "../../Mutex.h"
#include "../../Thread.h"
#include <set>
#include <stdio.h>
#include <boost/shared_ptr.hpp>

class Request;

class Inventory
{
public:
    Inventory()
        : requests_(new RequestList)
    {

    }

    void add(Request* req)
    {
        muduo::MutexLockGuard lock(mutex_);
        if (!requests_.unique())
        {
            requests_.reset(new RequestList(*requests_));
            printf("Inventory::add() copy the whole list\n");
        }
        assert(requests_.unique());
        requests_->insert(req);
    }

    void remove(Request* req)
    {
        muduo::MutexLockGuard lock(mutex_);
        if (!requests_.unique())
        {
            requests_.reset(new RequestList(*requests_));
            printf("Inventory::remove() copy the whole list\n");
        }
        assert(requests_.unique());
        requests_->erase(req);
    }

    void printAll() const;

private:
    typedef std::set<Request*> RequestList;
    typedef boost::shared_ptr<RequestList> RequestListPtr;

    RequestListPtr get_data() const
    {
        muduo::MutexLockGuard lock(mutex_);
        return requests_;
    }

    mutable muduo::MutexLock mutex_;
    RequestListPtr requests_;
};

Inventory g_inventory;

class Request
{
public:
    Request()
        : x_(0)
    {
    }

    ~Request() __attribute__ ((noinline))
    {
        muduo::MutexLockGuard lock(mutex_);
        x_ = -1;
        sleep(1);
        g_inventory.remove(this);
    }

    void process()
    {
        muduo::MutexLockGuard lock(mutex_);
        g_inventory.add(this);
    }

    void print() const __attribute__ ((noinline))
    {
        muduo::MutexLockGuard lock(mutex_);
        printf("print Request %p = %d!\n", this, x_);
    }

private:
    mutable muduo::MutexLock mutex_;
    int x_;
};

void Inventory::printAll() const
{
    RequestListPtr requests = get_data();
    sleep(1);
    for (std::set<Request*>::const_iterator it = requests->begin();
        it != requests->end(); ++it)
    {
        (*it)->print();
    }
}

// void Inventory::printAll() const
// {
//     std::set<Request*> requests;
//     {
//         muduo::MutexLockGuard lock(mutex_);
//         requests = requests_;
//     }
//     for (std::set<Request*>::const_iterator it = requests.begin();
//         it != requests.end(); ++it)
//     {
//         (*it)->print();
//     }
//     printf("Inventory::printAll() unlocked!\n");
// }

void threadFunc()
{
    Request* req = new Request;
    req->process();
    delete req;
}

int main()
{
    muduo::Thread thread(threadFunc);
    thread.start();
    usleep(500 * 1000);
    g_inventory.printAll();
    thread.join();
    printf("end...\n");
}