#ifndef MANAGER_H
#define MANAGER_H
#define STOCKS_SIZE 1009
#include <string>
#include "Stock.h"

class Manager{
public:
    Manager();
    ~Manager();
    Stock* stocks[STOCKS_SIZE] = {nullptr};
    void add_stock(std::string name, std::string wkn);
    void print_stocks();
    std::string stock_toupper(std::string characters);
    std::string get_initials(std::string name);
    int hash_function(std::string initials);
    Stock* find_stock(std::string name);


private:
    int stocksCount;
};

#endif // MANAGER_H
