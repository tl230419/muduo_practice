#include "../../Singleton.h"
#include "Foo.h"

int main()
{
    Foo& foo = Singleton<Foo>::instance();
    return 0;
}