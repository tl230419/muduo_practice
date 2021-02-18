#include "../../Mutex.h"

class Request
{
public:
    void process()
    {
        muduo::MutexLockGuard lock(mutex_);
        //print();
        printWithLockHold();
    }

    void print() const
    {
        muduo::MutexLockGuard lock(mutex_);
        printWithLockHold();
    }

    void printWithLockHold() const
    {
        printf("just test!\n");
    }

private:
    mutable muduo::MutexLock mutex_;
};

int main()
{
    Request req;
    req.process();
    req.print();
}