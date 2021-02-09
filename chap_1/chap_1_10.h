#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

class Foo
{
    Foo(std::string msg)
    {

    }

    void doit()
    {

    }
};

boost::shared_ptr<Foo> pFoo(new Foo);
boost::function<void()> func = boost::bind(&Foo::doit, pFoo);

void save(const boost::shared_ptr<Foo>& pFoo);
void validateAccount(const Foo& foo);

bool validate(const boost::shared_ptr<Foo>& pFoo)
{
    validateAccount(*pFoo);
    // ...
}

void onMessage(const std::string& msg)
{
    boost::shared_ptr<Foo> pFoo(new Foo(msg));
    if (validate(pFoo))
    {
        save(pFoo);
    }
}