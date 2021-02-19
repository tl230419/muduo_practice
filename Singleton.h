#ifndef SINGLETON_H
#define SINGLETON_H

#include <boost/noncopyable.hpp>
#include <pthread.h>
#include <stdlib.h>

template<typename T>
class Singleton : boost::noncopyable
{
public:
    static T& instance()
    {
        pthread_once(&ponce_, &Singleton::init);
        return *value_;
    }

private:
    Singleton();
    ~Singleton();

    static void init()
    {
        value_ = new T();
        ::atexit(destroy);
    }

    static void destroy()
    {
        delete value_;
    }

private:
    static pthread_once_t ponce_;
    static T* value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;

#endif // SINGLETON_H