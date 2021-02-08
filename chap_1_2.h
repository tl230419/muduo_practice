#ifndef FOO_H
#define FOO_H

// class Foo : public Observer
// {
// public:
//     Foo(Observer* s)
//     {
//         s->register_(this);
//     }

//     virtual void update();
// }

class Foo
{
public:
    Foo();
    ~Foo();

    virtual void update();

    void observe(Observer* s)
    {
        s->register_(this);
    }
};

#endif