#include "foo.h"
#include "observable.h"

int main(int argc, char**argv)
{
    Foo* pfoo = new Foo;
    Observable* s = new Observable;
    pfoo->observe(s);
    return 0;
}