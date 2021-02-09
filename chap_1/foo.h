#ifndef FOO_H
#define FOO_H

#include "observable.h"
#include "observer.h"
#include "mutexlock.h"

// class Foo : public Observer
// {
// public:
//     Foo(Observer* s)
//     {
//         s->register_(this);
//     }

//     virtual void update();
// }

class Foo : public Observer
{
public:
    Foo();
    ~Foo();

    virtual void update();

    void observe(Observable* s)
    {
        s->register_(this);
    }

private:
    MutexLock mutex_;
};

#endif