#include <boost/noncopyable.hpp>
#include "mutexlock.h"

// A thread-safe counter
class Counter : boost::noncopyable
{
    // copy-ctor and assignment should be private by default for a class.
public:
    Counter() : value_(0) {}

    int64_t value() const;
    void set_value(int64_t value);
    int64_t get_and_increase();

    MutexLock& get_mutex()
    {
        return mutex_;
    }

    Counter& Counter::operator=(const Counter& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        MutexLockGuard myLock(mutex_); // potential dead lock
        MutexLockGuard itsLock(rhs.mutex_);
        value_ = rhs.value_;
        return *this;
    }

private:
    int64_t value_;
    mutable MutexLock mutex_;
};

int64_t Counter::value() const
{
    MutexLockGuard lock(mutex_);
    return value_;
}

void Counter::set_value(int64_t value)
{
    value_ = value;
}

int64_t Counter::get_and_increase()
{
    MutexLockGuard lock(mutex_);
    int64_t ret = value_++;
    return ret;
}