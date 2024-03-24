#ifndef STOCK_H
#define STOCK_H
#include <string>

class Stock{
public:
    Stock(std::string name, std::string initials, std::string wkn);
    ~Stock();
    std::string get_name();
    std::string get_wkn();
    std::string get_initials();
private:
    std::string name;
    std::string wkn;
    std::string initials;
};

#endif // STOCK_H
