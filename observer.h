#ifndef OBSERVER_H
#define OBSERVER_H

class Observer // boost::noncopyable
{
public:
    Observer();
    virtual ~Observer();

    virtual void update();
};

#endif