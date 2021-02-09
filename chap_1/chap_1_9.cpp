#include "mutexlock.h"
#include <boost/shared_ptr.hpp>
#include "foo.h"

MutexLock mutex;
boost::shared_ptr<Foo> globalPtr;

void doit(const boost::shared_ptr<Foo>& pFoo)
{

}

void read()
{
    boost::shared_ptr<Foo> localPtr;
    {
        MutexLockGuard lock(mutex);
        localPtr = globalPtr; // read
    }
    doit(localPtr);
}

void write()
{
    boost::shared_ptr<Foo> newPtr(new Foo);
    {
        MutexLockGuard lock(mutex);
        globalPtr = newPtr; // write
    }
    doit(newPtr);
}

int main()
{
    return 0;
}