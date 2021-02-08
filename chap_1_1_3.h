#include <boost/noncopyable.hpp>
#include "mutexlock.h"

// A thread-safe counter
class Counter : boost::noncopyable
{
    // copy-ctor and assignment should be private by default for a class.
public:
    Counter() : value_(0) {}

    int64_t value() const;
    int64_t get_and_increase();

private:
    int64_t value_;
    mutable MutexLock mutex_;
};

int64_t Counter::value() const
{
    MutexLockGuard lock(mutex_);
    return value_;
}

int64_t Counter::get_and_increase()
{
    MutexLockGuard lock(mutex_);
    int64_t ret = value_++;
    return ret;
}