#include "CountDownLatch.h"

CountDownLatch::CountDownLatch(int count)
    : mutex_()
    , condition_(mutex_)
    , count_(count)
{

}

void CountDownLatch::wait()
{
    muduo::MutexLockGuard lock(mutex_);
    while (count_ > 0)
    {
        condition_.wait();
    }
}
void CountDownLatch::count_down()
{
    muduo::MutexLockGuard lock(mutex_);
    --count_;
    if (count_ == 0)
    {
        condition_.notifyAll();
    }
}