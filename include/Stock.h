#ifndef STOCK_H
#define STOCK_H
#include <string>
#include "MarketData.h"
#define MAXIMUM_MARKET_DATA 30
#include <sstream>
#include <iomanip>

class Stock
{
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
    std::string get_last_market_data();
    std::string get_market_data();
    int get_market_data_capacity();
    int get_market_data_count();
    void delete_market_data();
    bool market_data_exists(std::string date);
    std::string serialize() const;

private:
    std::string name;
    std::string wkn;
    std::string initials;
    int marketDataCount;


};

#endif // STOCK_H
