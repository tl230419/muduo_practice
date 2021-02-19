#ifndef FOO_H
#define FOO_H

#include <stdio.h>

class Foo
{
public:
    Foo()
    {
        printf("init...\n");
    }
    
    ~Foo()
    {

    }
};

#endif // FOO_H