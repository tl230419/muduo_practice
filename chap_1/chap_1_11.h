#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include "mutexlock.h"
#include <map>

class Stock;

MutexLock mutex_;

// version 1: questionable code
class StockFactory : public boost::enable_shared_from_this<StockFactory>,
                     boost::noncopyable
{
public:
    boost::shared_ptr<Stock> get(const std::string& key);

private:
    void deleteStock(Stock* stock)
    {
        if (stock)
        {
            MutexLockGuard lock(mutex_);
            stocks_.erase(stock->key());
        }
        delete stock;
    }

private:
    mutable MutexLock mutex_;
    std::map<std::string, boost::shared_ptr<Stock>> stocks_;
};

boost::shared_ptr<Stock> StockFactory::get(const std::string& key)
{
    boost::shared_ptr<Stock> pStock;
    MutexLockGuard lock(mutex_);
    boost::weak_ptr<Stock>& wkStock = stocks_[key];
    pStock = wkStock.lock();
    if (!pStock)
    {
        //pStock.reset(new Stock(key));
        // pStock.reset(new Stock(key), 
        //             boost::bind(&StockFactory::deleteStock, this, _1));
        pStock.reset(new Stock(key), 
                    boost::bind(&StockFactory::deleteStock, shared_from_this(), _1));
        wkStock = pStock;
    }

    return pStock;
}

class Stock
{
public:
    Stock(std::string key);
    ~Stock();

    std::string key();

private:
    std::string key;
};

boost::shared_ptr<StockFactory> stockFactory(new StockFactory);