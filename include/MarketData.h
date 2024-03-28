#ifndef MARKETDATA_H
#define MARKETDATA_H
#include <string>


class MarketData
{
public:
    MarketData(std::string date,
               double open,
               double high,
               double low,
               double close,
               double volume,
               double adjClose);
    virtual ~MarketData();
    std::string get_date();
    double get_open();
    double get_high();
    double get_low();
    double get_close() const;
    double get_volume();
    double get_adjClose();

private:
    std::string date;
    double open;
    double high;
    double low;
    double close;
    double volume;
    double adjClose;
};

#endif // MARKETDATA_H
