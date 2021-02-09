#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include "mutexlock.h"
#include <map>
#include <assert.h>

class Stock;
MutexLock mutex_;

class StockFactory : public boost::enable_shared_from_this<StockFactory>,
                     boost::noncopyable
{
public:
    boost::shared_ptr<Stock> get(const std::string& key)
    {
        boost::shared_ptr<Stock> pStock;
        MutexLockGuard lock(mutex_);
        boost::weak_ptr<Stock>& wkStock = stocks_[key];
        pStock = wkStock.lock();
        if (!pStock)
        {
            pStock.reset(new Stock(key), 
                        boost::bind(&StockFactory::weakDeleteCallback, 
                                    boost::weak_ptr<StockFactory>(shared_from_this()), 
                                    _1));
            wkStock = pStock;
        }

        return pStock;
    }

private:
    static void weakDeleteCallback(const boost::weak_ptr<StockFactory>& wkFactory, Stock* stock)
    {
        boost::shared_ptr<StockFactory> factory(wkFactory.lock());
        if (factory)
        {
            factory->removeStock(stock);
        }
        delete stock;
    }

    void removeStock(Stock* stock)
    {
        if (stock)
        {
            MutexLockGuard lock(mutex_);
            stocks_.erase(stock->key());
        }
    }

private:
    mutable MutexLock mutex_;
    std::map<std::string, boost::weak_ptr<Stock>> stocks_;
};

class Stock
{
public:
    Stock(const std::string& key)
        : key_(key)
    {
    }

    ~Stock()
    {
    }

    const std::string& key()
    {
        return key_;
    }

private:
    std::string key_;
};