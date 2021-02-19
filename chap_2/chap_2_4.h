#include <boost/noncopyable.hpp>
#include "../Mutex.h"
#include <pthread.h>

class Condition : boost::noncopyable
{
public:
    explicit Condition(muduo::MutexLock& mutex)
        : mutex_(mutex)
    {
        pthread_cond_init(&pcond_, NULL);
    }

    ~Condition()
    {
        pthread_cond_destroy(&pcond_);
    }

private:
    muduo::MutexLock& mutex_;
    pthread_cond_t pcond_;
};