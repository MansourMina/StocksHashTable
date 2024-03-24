#ifndef MANAGER_H
#define MANAGER_H
#define STOCKS_SIZE 2003
#include <string>
#include "Stock.h"
#include "HashTable.h"
enum SEARCH_TYPE
{
    BY_NAME= 1,
    BY_INITIALS
};

class Manager
{
public:
    Manager();
    ~Manager();
    Stock* stocks[STOCKS_SIZE] = {nullptr};
    void add_stock(std::string name, std::string wkn);
    //void print_stocks();
    std::string stock_toupper(std::string characters);
    std::string get_initials(std::string name);
    HashTable * hashtable;
    Stock* search_stock(int searchType, std::string searchKey);

private:
    int stocksCount;
};

#endif // MANAGER_H
