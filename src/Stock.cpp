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
    delete_market_data();
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

int Stock::get_market_data_capacity()
{
    return MAXIMUM_MARKET_DATA;
}

int Stock::get_market_data_count()
{
    return marketDataCount;
}

void Stock::delete_market_data(){
    for(int m = 0; m < marketDataCount; m++)
        delete marketData[m];
    marketDataCount = 0;
}

std::string Stock::get_market_data()
{
    std::stringstream ss;
    ss << "Date | Open | High | Low | Close | Adj Close | Volume\n" << std::endl;
    for(int i = 0; i < marketDataCount; i++)
    {
        MarketData* md = marketData[i];
        ss << md->get_date() << " | "
           << std::fixed << std::setprecision(2) << md->get_open() << " | "
           << std::fixed << std::setprecision(2) << md->get_high() << " | "
           << std::fixed << std::setprecision(2) << md->get_low() << " | "
           << std::fixed << std::setprecision(2) << md->get_close() << " | "
           << std::fixed << std::setprecision(2) << md->get_adjClose() << " | "
           << std::fixed << std::setprecision(2) << md->get_volume()
           << std::endl;
    }
    return ss.str();
}

bool Stock::market_data_exists(std::string date){
    for(int d = 0; d < marketDataCount; d++){
        if(marketData[d]->get_date() == date) return true;
    }
    return false;
}


