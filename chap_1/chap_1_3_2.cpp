#include "counter.h"
#include "mutexlock.h"

Counter a;
Counter b;

void swap(Counter& a, Counter& b)
{
    MutexLockGuard aLock(a.get_mutex()); // potential dead lock
    MutexLockGuard bLock(b.get_mutex());
    int64_t value = a.value();
    a.set_value(b.value());
    b.set_value(a.value());
}