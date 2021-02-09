#include "foo.h"

extern Foo* x; // visible by all threads

int main(int argc, char**argv)
{
    // thread A
    delete x;
    x = NULL;   // helpless

    // thread B
    if (x)
    {
        x->update();
    }
    
    return 0;
}