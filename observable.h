#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "observer.h"
#include <vector>

class Observable // boost::noncopyable
{
public:
    Observable();
    ~Observable();

    void register_(Observer* s);
    void unregister(Observer* s);

    void notifyObservers()
    {
        for (Observer* x : observers_)
        {
            x->update();
        }
    }

private:
    std::vector<Observer*> observers_;
};


#endif