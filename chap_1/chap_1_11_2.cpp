#include "chap_1_11_2.h"

void testLongLifeFactory()
{
    boost::shared_ptr<StockFactory> factory(new StockFactory);
    {
        boost::shared_ptr<Stock> stock = factory->get("NYSE::IBM");
        boost::shared_ptr<Stock> stock2 = factory->get("NYSE::IBM");
        assert(stock == stock2);
        // stock destructs here
    }
    // factory destructs here
}

void testShortLifeFactory()
{
    boost::shared_ptr<Stock> stock;
    {
        boost::shared_ptr<StockFactory> factory(new StockFactory);
        stock = factory->get("NYSE::IBM");
        boost::shared_ptr<Stock> stock2 = factory->get("NYSE::IBM");
        assert(stock == stock2);
        // factory destructs here
    }
    // stock destructs here
}

int main()
{
    testLongLifeFactory();
    testShortLifeFactory();

    return 0;
}