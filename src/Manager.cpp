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

void Manager::add_market_data(std::string file, std::string term)
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
    sort_data(file);
    import_data(file, stocks[hashIndex]);
    stocks[hashIndex]->print_market_data();
}

bool Manager::stock_exists(std::string term)
{
    term = stock_toupper(term);
    if ((*name_initials).find(term) != (*name_initials).end())
        term = (*name_initials)[term];

    int hashIndex = hashtable->find_hash_index(stocks, term);
    return !(hashIndex == -1);
}


bool Manager::compareByDate(const file_data& a, const file_data& b)
{
    return a.date > b.date;
}

void Manager::sort_data(std::string& filename)
{
    std::ifstream infile(filename);
    std::vector<file_data> data;
    std::string line;
    getline(infile, line); // Skip header
    while (std::getline(infile, line))
    {
        std::stringstream ss(line);
        file_data fdata;
        std::getline(ss, fdata.date, ',');
        ss >> fdata.open >> fdata.high >> fdata.low >> fdata.close >> fdata.adjClose >> fdata.volume;
        data.push_back(fdata);
    }
    infile.close();

    // Sort data
    std::sort(data.begin(), data.end(), [this](const file_data& a, const file_data& b)
    {
        return compareByDate(a, b);
    });

    // Write sorted data back to file
    std::ofstream outfile(filename);

    outfile << "Date,Open,High,Low,Close,AdjClose,Volume\n";
    for (int i = 0; i < data.size(); ++i)
    {
        outfile << data[i].date << "," << data[i].open << "," << data[i].high << "," << data[i].low << "," << data[i].close << "," << data[i].adjClose << "," << data[i].volume << "\n";
    }
    outfile.close();
}

void Manager::import_data(std::string file, Stock* stock)
{

    std::string date,open,high,low,close,adjClose,volume;
    std::ifstream stockData;
    stockData.open(file);
    if (!stockData.is_open())
    {
        std::cerr << "Error: Failed to open file!" << std::endl;
        return;
    }

    std::string column;

    int counter= 0, beginWith = 1;
    bool dataLost = false;

    while (getline(stockData,column))
    {
        if(counter >= beginWith)
        {
            std::stringstream ss(column);
            std::getline(ss, date, ',');
            std::getline(ss, open, ',');
            std::getline(ss, high, ',');
            std::getline(ss, low, ',');
            std::getline(ss, close, ',');
            std::getline(ss, adjClose, ',');
            std::getline(ss, volume, ',');

            double openValue = (open.empty() ? 0.00 : std::stod(open));
            double highValue = (high.empty() ? 0.00 : std::stod(high));
            double lowValue = (low.empty() ? 0.00 : std::stod(low));
            double closeValue = (close.empty() ? 0.00 : std::stod(close));
            double adjCloseValue = (adjClose.empty() ? 0.00 : std::stod(adjClose));
            double volumeValue = (volume.empty() ? 0.00 : std::stod(volume));

            dataLost = counter > stock->get_market_data_capacity();
            if(dataLost) break;
            stock->add_market_data(date, openValue, highValue, lowValue, closeValue, volumeValue, adjCloseValue);
        }
        counter++;
    }
    std::cout << "Market data added to Stock '" << stock->get_name() << "': " << stock->get_market_data_count() << std::endl;
    if(dataLost) std::cerr << "Warning: Market data lost. Exceeded maximum capacity of " << stock->get_market_data_capacity() << std::endl;
    stockData.close();

}
