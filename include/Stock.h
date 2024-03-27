#ifndef STOCK_H
#define STOCK_H
#include <string>
#include "MarketData.h"
#define MAXIMUM_MARKET_DATA 30

class Stock{
public:
    Stock(std::string name, std::string initials, std::string wkn);
    ~Stock();
    std::string get_name();
    std::string get_wkn();
    std::string get_initials();
    MarketData* marketData[MAXIMUM_MARKET_DATA];
    void add_market_data(std::string date,
                            double open,
                            double high,
                            double low,
                            double close,
                            double volume,
                            double adjClose);
    void print_market_data();
    int get_market_data_capacity();
    int get_market_data_count();
private:
    std::string name;
    std::string wkn;
    std::string initials;
    int marketDataCount;

};

#endif // STOCK_H
