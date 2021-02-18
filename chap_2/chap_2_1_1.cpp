#include "../Mutex.h"
#include "../Thread.h"
#include <vector>
#include <stdio.h>

using namespace muduo;

class Foo
{
public:
    void doit() const;
};

MutexLock mutex;
std::vector<Foo> foos;

void post(const Foo& f)
{
    MutexLockGuard lock(mutex);
    foos.push_back(f);
    printf("saved.\n");
}

void postWithLockHold(const Foo& f)
{
    foos.push_back(f);
}

void post1(const Foo& f)
{
    MutexLockGuard lock(mutex);
    postWithLockHold(f);
}

void traverse()
{
    MutexLockGuard lock(mutex);
    for (std::vector<Foo>::const_iterator it = foos.begin();
        it != foos.end(); ++it)
    {
        it->doit();
    }
}

void Foo::doit() const
{
    Foo f;
    post(f);
}

int main()
{
    Foo f;
    post(f);
    traverse();
    return 0;
}