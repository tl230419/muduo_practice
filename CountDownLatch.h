#include <boost/noncopyable.hpp>
#include "../../Mutex.h"
#include "../../Condition.h"

class CountDownLatch : boost::noncopyable
{
public:
    explicit CountDownLatch(int count);
    void wait();
    void count_down();

private:
    mutable muduo::MutexLock mutex_;
    muduo::Condition condition_;
    int count_;
};