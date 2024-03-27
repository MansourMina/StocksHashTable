#include "Stock.h"
#include <string>
#include <iostream>

Stock::Stock(std::string name, std::string initials, std::string wkn)
{
    this->name = name;
    this->initials= initials;
    this->wkn = wkn;
    marketDataCount = 0;
}

Stock::~Stock()
{
}

std::string Stock::get_name()
{
    return name;
}

std::string Stock::get_wkn()
{
    return wkn;
}

std::string Stock::get_initials()
{
    return initials;
}

void Stock::add_market_data(std::string date,
                            double open,
                            double high,
                            double low,
                            double close,
                            double volume,
                            double adjClose)
{
    if(marketDataCount + 1 > MAXIMUM_MARKET_DATA) return;
    marketData[marketDataCount++] = new MarketData(date, open, high, low, close, volume, adjClose);
}

int Stock::get_market_data_capacity(){
    return MAXIMUM_MARKET_DATA;
}

int Stock::get_market_data_count(){
    return marketDataCount;
}

void Stock::print_market_data(){
    for(int i = 0; i < marketDataCount; i++){
        std::cout << marketData[i]->get_date() << std::endl;
    }
}


