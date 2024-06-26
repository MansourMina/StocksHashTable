#include "MarketData.h"
#include <iostream>

MarketData::MarketData(std::string date,
                       double open,
                       double high,
                       double low,
                       double close,
                       double adjClose,
                       double volume
                       )
{
    this->date = date;
    this->open = open;
    this->high = high;
    this->low = low;
    this->close = close;
    this->volume = volume;
    this->adjClose = adjClose;
}

MarketData::~MarketData()
{
    //dtor
}


std::string MarketData::get_date()
{
    return date;
}
double MarketData::get_open()
{
    return open;
}
double MarketData::get_high()
{
    return high;
}
double MarketData::get_low()
{
    return low;
}
double MarketData::get_close() const
{
    return close;
}
double MarketData::get_volume()
{
    return volume;
}
double MarketData::get_adjClose()
{
    return adjClose;
}
