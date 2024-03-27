#ifndef MANAGER_H
#define MANAGER_H
#define STOCKS_SIZE 1301
#include <string>
#include "Stock.h"
#include "HashTable.h"
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

enum SEARCH_TYPE
{
    BY_NAME= 1,
    BY_INITIALS
};

struct file_data
{
    std::string date;
    double open;
    double high;
    double low;
    double close;
    double adjClose;
    double volume;
};

class Manager
{
public:
    Manager();
    ~Manager();
    Stock* stocks[STOCKS_SIZE] = {nullptr};
    void add_stock(std::string name, std::string initials, std::string wkn);
    void print_stock(Stock* stock);
    std::string stock_toupper(std::string characters);
    //std::string get_initials(std::string name);
    HashTable * hashtable;
    Stock* search_stock(std::string term);
    std::unordered_map<std::string, std::string>* name_initials;
    std::unordered_set<std::string>* initials;
    bool name_exists(std::string name);
    bool initials_exists(std::string initials);
    void del_stock(std::string term);
    void del_map_initials(std::string key);
    void del_map_name_initials(std::string key);
    void add_market_data(std::string file, std::string term);
    bool stock_exists(std::string term);

private:
    int stocksCount;
    void import_data(std::string file, Stock* stock);
    int count_file_lines(std::ifstream& file);
    bool compareByDate(const file_data& a, const file_data& b);
    void sort_data(std::string& filename);
};

#endif // MANAGER_H
