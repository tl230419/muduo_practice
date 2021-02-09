#include "foo.h"

Foo::Foo()
{

}

Foo::~Foo()
{
    MutexLockGuard lock(mutex_);
    // free internal state (1)
}

void Foo::update()
{
    MutexLockGuard lock(mutex_); // (2)
    // make use of internal state
}