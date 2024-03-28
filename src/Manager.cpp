#include "Manager.h"
#include <iostream>
#include <cmath>


Manager::Manager()
{
    stocksCount= 0;
    hashtable = new HashTable(STOCKS_SIZE);
    name_initials = new std::unordered_map<std::string, std::string>;
    initials = new std::unordered_set<std::string>;
}

Manager::~Manager()
{
    for(int i = 0; i < stocksCount; i++)
        delete stocks[i];
    delete name_initials;
    delete initials;
}

void Manager::add_stock(std::string name,std::string initials, std::string wkn)
{
    if(stocksCount + 1 >= STOCKS_SIZE)
    {
        std::cerr << "Failed: Maximum stock capacity reached!" << std::endl;
        return;
    }
    name = stock_toupper(name), initials = stock_toupper(initials);
    int hashIndex = hashtable->hash_function(stocks, initials);
    stocks[hashIndex] = new Stock(name, initials, wkn);
    (*name_initials)[name] = initials;
    (*this->initials).insert(initials);
    stocksCount++;
}

void Manager::del_stock(std::string term)
{
    term = stock_toupper(term);
    if ((*name_initials).find(term) != (*name_initials).end())
        term = (*name_initials)[term];

    int hashIndex = hashtable->find_hash_index(stocks, term);
    if(hashIndex == -1)
    {
        std::cerr << "Failed: Stock not found!" << std::endl;
        return;
    }
    del_map_initials(stocks[hashIndex]->get_initials());
    del_map_name_initials(stocks[hashIndex]->get_name());
    delete stocks[hashIndex];
    stocks[hashIndex] = nullptr;
    std::cout << "Success: Stock deleted!" << std::endl;

}

void Manager::del_map_initials(std::string key)
{

    (*initials).erase((*initials).find(key));
}

void Manager::del_map_name_initials(std::string key)
{

    (*name_initials).erase(key);
}

Stock* Manager::search_stock(std::string term)
{
    term = stock_toupper(term);
    if ((*name_initials).find(term) != (*name_initials).end())
        term = (*name_initials)[term];

    int found_hashIndex = hashtable->find_hash_index(stocks, term);
    return found_hashIndex == -1 ? nullptr:stocks[found_hashIndex];
}


//std::string Manager::get_initials(std::string name)
//{
//    if(name.length() <= 3) return name;
//
//    char initials[5];
//    float position = 0;
//    for(int i = 0; i < std::sqrt(name.length()); i++)
//    {
//        initials[i] = name[name.length() * position];
//        position += 0.25;
//    }
//    return initials;
//}

std::string Manager::stock_toupper(std::string characters)
{
    for(int i = 0; i < characters.length(); i++)
        characters[i] = std::toupper(characters[i]);

    return characters;
}

void Manager::print_stock(Stock* stock)
{
    std::cout << "\n";
    std::cout << "Name: " << stock->get_name() << std::endl;
    std::cout << "Initials: " << stock->get_initials() << std::endl;
    std::cout << "WKN: " << stock->get_wkn() << std::endl;
    std::cout << "\n";
    if(stock->get_market_data_count() > 0)
        std::cout << stock->get_market_data() << std::endl;
}

bool Manager::name_exists(std::string name)
{
    name = stock_toupper(name);
    return (*name_initials).find(name) != (*name_initials).end();
}

bool Manager::initials_exists(std::string initials)
{
    initials = stock_toupper(initials);
    return (*this->initials).find(initials) != (*this->initials).end();
}

bool Manager::stock_exists(std::string term)
{
    term = stock_toupper(term);
    if ((*name_initials).find(term) != (*name_initials).end())
        term = (*name_initials)[term];

    int hashIndex = hashtable->find_hash_index(stocks, term);
    return !(hashIndex == -1);
}

void Manager::add_market_data(std::string file, std::string term, int importType)
{
    Stock* stock = search_stock(term);

    std::ifstream input_file(file);
    if (!input_file)
    {
        std::cerr << "Error: Failed to open file!" << std::endl;
        return;
    }

    std::vector<file_data> data;
    read_data(input_file, data);
    sort_data(data, 0, data.size() - 1);
    import_data(data, stock, importType);
}


void Manager::sort_data(std::vector<file_data>& data, int low, int high)
{
    if (low < high)
    {
        int pivotIndex = low + (high - low) / 2;
        file_data pivot = data[pivotIndex];
        int i = low;
        int j = high;
        while (i <= j)
        {
            while (data[i].date > pivot.date)
                i++;
            while (pivot.date > data[j].date)
                j--;
            if (i <= j)
            {
                std::swap(data[i], data[j]);
                i++;
                j--;
            }
        }
        sort_data(data, low, j);
        sort_data(data, i, high);
    }
}

double Manager::convert_to_double(std::string number)
{
    return number.empty() ? 0.00: std::stod(number);
}

void Manager::read_data(std::ifstream& input_file, std::vector<file_data>& data)
{
    if (input_file)
    {
        std::string line;
        int lineCounter= 0, beginWith = 1;
        while (std::getline(input_file, line))
        {
            if(lineCounter >= beginWith)
            {
                std::stringstream ss(line);
                file_data currData;
                std::string column;
                std::getline(ss, currData.date, ',');
                std::getline(ss, column, ',');
                currData.open =  convert_to_double(column);
                std::getline(ss, column, ',');
                currData.high = convert_to_double(column);
                std::getline(ss, column, ',');
                currData.low = convert_to_double(column);
                std::getline(ss, column, ',');
                currData.close = convert_to_double(column);
                std::getline(ss, column, ',');
                currData.adjClose = convert_to_double(column);
                std::getline(ss, column);
                currData.volume = convert_to_double(column);
                data.push_back(currData);
            }
            lineCounter++;
        }
        input_file.close();
    }
}

void Manager::import_data(std::vector<file_data>& data, Stock* stock, int importType )
{
    if(importType == IMPORT_TYPE::REPLACING)
        stock->delete_market_data();

    bool dataLost = false, dublicates= false;
    int added= 0;
    int capacity = stock->get_market_data_capacity() - stock->get_market_data_count();
    for (int dataCounter = 0; dataCounter < data.size(); dataCounter++)
    {
        const file_data& currData = data[dataCounter];
        dataLost =  dataCounter >= capacity;
        if(dataLost) break;
        if(stock->market_data_exists(currData.date))
        {
            dublicates= true;
            continue;
        }
        stock->add_market_data(currData.date, currData.open, currData.high, currData.low, currData.close, currData.volume, currData.adjClose);
        added++;
    }
    std::cout << "\n";
    std::cout << "Market data added to Stock '" << stock->get_name() << "': " << added << std::endl;
    if(dataLost) std::cerr << "Warning: Market data lost. Exceeded maximum capacity of " << stock->get_market_data_capacity() << std::endl;
    if(dublicates) std::cerr << "Warning: Duplicate Market data detected." << std::endl;
    std::cout << "\n";
}

double Manager::get_max_closed(Stock* stock)
{
    if(stock->get_market_data_count() <= 0) return 0.00;
    double currentMax = stock->marketData[0]->get_close();
    for(int i = 0; i < stock->get_market_data_count(); i++)
    {
        if(stock->marketData[i]->get_close() > currentMax)
        {
            currentMax = stock->marketData[i]->get_close();
        }
    }
    return currentMax;
}

double Manager::get_min_closed(Stock* stock)
{
    if(stock->get_market_data_count() <= 0) return 0.00;
    double currentMin= stock->marketData[0]->get_close();
    for(int i = 0; i < stock->get_market_data_count(); i++)
    {
        if(stock->marketData[i]->get_close() < currentMin)
        {
            currentMin = stock->marketData[i]->get_close();
        }
    }
    return currentMin;
}

void Manager::plot_market_data(Stock* stock)
{

    double maxClose = get_max_closed(stock);
    double minClose = get_min_closed(stock);
    int dataCount = stock->get_market_data_count();

    std::vector<double> closeData;
    for(int i = 0; i < dataCount; i++)
    {
        closeData.push_back(stock->marketData[i]->get_close());
    }
    std::sort(closeData.begin(), closeData.end());
    std::reverse(closeData.begin(), closeData.end());

    std::cout << "Close" << std::endl;

    for(int c = 0; c < closeData.size(); c++)
    {
        std::cout << std::fixed << std::setprecision(2) << closeData[c] << " |";
        for(int i = 0; i < dataCount; i++)
        {
            std::istringstream iss(stock->marketData[i]->get_date());

            double currStockClose = stock->marketData[i]->get_close();
            std::cout << (currStockClose >= closeData[c] ? "*  " : "   ");
        }
        std::cout << "\n";
    }

    for(int j = 0; j < std::to_string(maxClose).length() - 2; j++)
            std::cout << "-";
    for(int i = 0; i < dataCount; i++)
    {

        std::istringstream iss(stock->marketData[i]->get_date());
        int year, month, day;
        char dash;
        iss >> year >> dash >> month >> dash >> day;
        std::cout  << (month < 10 ? "0" + std::to_string(month) : std::to_string(month)) << "-";
    }
    std::cout << "----Month" << std::endl;


}



