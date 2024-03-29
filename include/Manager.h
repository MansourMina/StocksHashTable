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
#include <regex>

enum SEARCH_TYPE
{
    BY_NAME= 1,
    BY_INITIALS
};

enum IMPORT_TYPE
{
    REPLACING=1,
    ADDING
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

    void parse_line(std::string& line)
    {
        std::stringstream ss(line);
        std::string column;
        std::getline(ss, date, ',');
        std::getline(ss, column, ',');
        open = column.empty() ? 0.00: std::stod(column);
        std::getline(ss, column, ',');
        high = column.empty() ? 0.00: std::stod(column);
        std::getline(ss, column, ',');
        low = column.empty() ? 0.00: std::stod(column);
        std::getline(ss, column, ',');
        close = column.empty() ? 0.00: std::stod(column);
        std::getline(ss, column, ',');
        adjClose = column.empty() ? 0.00: std::stod(column);
        std::getline(ss, column);
        volume = column.empty() ? 0.00: std::stod(column);
    }
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
    void add_market_data(std::string file, std::string term, int importType);
    bool stock_exists(std::string term);
    void plot_market_data(Stock* stock);
    void save_data();
    void load_data(std::string filename);

    std::vector<int> storedIndexes;

private:
    int stocksCount;
    void import_data(std::vector<file_data>& data, Stock* stock, int importType);
    int count_file_lines(std::ifstream& file);
    void sort_data(std::vector<file_data>& data, int low, int high);
    void read_data(std::ifstream& input_file, std::vector<file_data>& data);
    bool is_date(std::string date);
    double get_max_closed(Stock* stock);
    bool valid_file(std::ifstream& file);
};

#endif // MANAGER_H
