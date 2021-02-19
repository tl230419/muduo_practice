#include "../../Mutex.h"
#include "../../Condition.h"
#include "../../Thread.h"
#include <deque>
#include <assert.h>

muduo::MutexLock    mutex;
muduo::Condition    cond(mutex);
std::deque<int>     queue;

int dequeue()
{
    muduo::MutexLockGuard lock(mutex);
    while (queue.empty())
    {
        printf("queue is empty\n");
        cond.wait();
    }
    assert(!queue.empty());
    int top = queue.front();
    queue.pop_front();
    printf("return value %d\n", top);
    return top;
}

void enqueue(int x)
{
    muduo::MutexLockGuard lock(mutex);
    queue.push_back(x);
    printf("put value = %d\n", x);
    cond.notify();
    printf("queue not empty\n");
}

void thread_func()
{
    int ret = dequeue();
    printf("value = %d\n", ret);
}

int main()
{
    muduo::Thread thread(thread_func);
    thread.start();
    sleep(5);
    enqueue(100);
    thread.join();
    return 0;
}