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
               double adjClose,
               double volume
              );
    virtual ~MarketData();
    std::string get_date();
    double get_open();
    double get_high();
    double get_low();
    double get_close() const;
    double get_adjClose();
    double get_volume();


private:
    std::string date;
    double open;
    double high;
    double low;
    double close;
    double adjClose;
    double volume;

};

#endif // MARKETDATA_H
