#include "Manager.h"
#include <iostream>
#include <cmath>


Manager::Manager()
{
    stocksCount= 0;
    hashtable = new HashTable(HASH_TABLE_SIZE);
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
    if(stocksCount + 1 >= MAX_STOCKS)
    {
        std::cerr << "Failed: Maximum stock capacity reached!" << std::endl;
        return;
    }
    if(search_stock(initials) != nullptr) return;
    name = stock_toupper(name), initials = stock_toupper(initials);
    int hashIndex = hashtable->hash_function(stocks, initials);
    stocks[hashIndex] = new Stock(name, initials, wkn);
    (*name_initials)[name] = initials;
    this->initials->insert(initials);
    stocksCount++;
    storedIndexes.push_back(hashIndex);
    std::cout << "Successfully added Stock '" << stocks[hashIndex]->get_name() << "'" << std::endl;
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
    stocksCount--;
    auto index = std::find(storedIndexes.begin(), storedIndexes.end(), hashIndex);
    if (index != storedIndexes.end())
    {
        storedIndexes.erase(index);
    }
    std::cout << "Successfully deleted Stock!" << std::endl;


}

void Manager::del_map_initials(std::string key)
{

    initials->erase(initials->find(key));
}

void Manager::del_map_name_initials(std::string key)
{

    name_initials->erase(key);
}

Stock* Manager::search_stock(std::string term)
{
    term = stock_toupper(term);
    if ((*name_initials).find(term) != (*name_initials).end())
        term = (*name_initials)[term];

    int found_hashIndex = hashtable->find_hash_index(stocks, term);
    return found_hashIndex == -1 ? nullptr:stocks[found_hashIndex];
}


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
        std::cout << stock->get_last_market_data() << std::endl;
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

bool Manager::valid_file(std::ifstream& file)
{
    if (!file)
    {
        std::cerr << "Error: Failed to open file!" << std::endl;
        return false;
    }
    return true;
}

void Manager::add_market_data(std::string file, std::string term, int importType)
{
    Stock* stock = search_stock(term);
    std::ifstream input_file(file);
    if(!valid_file(input_file)) return;
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



void Manager::read_data(std::ifstream& input_file, std::vector<file_data>& data)
{
    std::string line;
    int lineCounter= 0, beginWith = 1;
    while (std::getline(input_file, line))
    {
        if(lineCounter >= beginWith)
        {
            std::stringstream ss(line);
            file_data currData;
            currData.parse_line(line);
            data.push_back(currData);
        }
        lineCounter++;
    }
    input_file.close();
}

void Manager::import_data(std::vector<file_data> data, Stock* stock, int importType )
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
        stock->add_market_data(currData.date, currData.open, currData.high, currData.low, currData.close, currData.adjClose, currData.volume);
        added++;
    }
    std::cout << "Market data imported into Stock '" << stock->get_name() << "': " << added << std::endl;
    if(dataLost) std::cerr << "Warning: Market data lost. Exceeded maximum capacity of " << stock->get_market_data_capacity() << std::endl;
    if(dublicates) std::cerr << "Warning: Duplicate Market data detected." << std::endl;
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

std::vector<double> Manager::get_y_close_data(int dataCount, Stock* stock)
{
    std::vector<double> closeData;
    for(int i = 0; i < dataCount; i++)
    {
        closeData.push_back(stock->marketData[i]->get_close());
    }
    std::sort(closeData.begin(), closeData.end());
    std::reverse(closeData.begin(), closeData.end());
    double lastData = closeData.back() - ((closeData[0] - closeData.back()) / closeData.size());
    closeData.push_back(lastData);
    return closeData;
}


void Manager::plot_market_data(Stock* stock)
{
    int dataCount = stock->get_market_data_count();
    if(dataCount <= 0)
    {
        std::cout << "'" << stock->get_name() << "'" << " has no Market data yet!" << std::endl;
        return;
    }

    double maxClose = get_max_closed(stock);
    std::vector<double> closeData= get_y_close_data(dataCount, stock);


    std::cout << "\nClose\n" << std::endl;
    for(int c = 0; c < closeData.size(); c++)
    {
        std::cout << std::fixed << std::setprecision(2) << closeData[c] << "  ";
        for(int i = 0; i < dataCount; i++)
        {
            std::istringstream iss(stock->marketData[i]->get_date());

            double currStockClose = stock->marketData[i]->get_close();
            if(currStockClose == closeData[c])std::cout << "______";
            else if(currStockClose > closeData[c]) std::cout << "|   | ";
            else std::cout << "______";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    for(int j = 0; j < std::to_string(maxClose).length() - 2; j++)
        std::cout << " ";
    for(int i = 0; i < dataCount; i++)
    {

        std::istringstream iss(stock->marketData[i]->get_date());
        int year, month, day;
        char dash;
        iss >> year >> dash >> month >> dash >> day;
        std::cout  << (month < 10 ? "0" + std::to_string(month) : std::to_string(month)) << "-" << (day < 10 ? "0" + std::to_string(day) : std::to_string(day)) << " ";
    }
    std::cout << "     Days" << std::endl;
    std::cout << "\n";
}

void Manager::save_data()
{
    const char* downloadsDir = std::getenv("USERPROFILE");
    if (downloadsDir == nullptr)
    {
        std::cout << "Failed: Could not save file. Please try again!" << std::endl;
        return;
    }
    std::string filename = std::string(downloadsDir) + "\\Downloads\\Stocks_data.csv";
    std::ofstream file(filename);
    bool isEmpty = true;
    for (int i = 0; i < storedIndexes.size(); ++i)
    {
        int hashIndex = storedIndexes[i];
        file << stocks[hashIndex]->serialize() << "\n";
        file << stocks[hashIndex]->get_market_data() << "\n";
        isEmpty = false;
    }
    file.close();
    std::cout << (isEmpty ? "Error: No valid data found!":"Successfully saved stock data!") << std::endl;

}

bool Manager::is_date(std::string date)
{
    if (date.length() != 10)
        return false;
    for (int i = 0; i < 10; ++i)
    {
        if (i == 4 || i == 7)
        {
            if (date[i] != '-' && date[i] != '/' && date[i] != '.')
                return false;
        }
        else
        {
            if (!std::isdigit(date[i]))
                return false;
        }
    }
    return true;
}

void Manager::load_data(std::string filename)
{
    std::ifstream file(filename);
    if(!valid_file(file)) return;
    std::vector<file_data> market_data;
    std::string line;
    bool isStockHeader = true, toAdd = false, imported= false;
    Stock* stock = nullptr;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        if(!toAdd && line.empty())
        {
            isStockHeader = true;
            continue;
        }
        if(isStockHeader)
        {
            std::string name, initials, wkn;
            std::getline(ss, name, ',');
            std::getline(ss, initials, ',');
            ss >> wkn;
            add_stock(name,initials,wkn);
            stock = search_stock(initials);
            if(stock == nullptr)
            {
                std::cout << "Warning: Could not import data!" << std::endl;
                return;
            }
            isStockHeader = false;
            imported = true;
            continue;
        }
        std::string date;
        std::getline(ss, date, ',');
        if (is_date(date))
        {
            std::stringstream ss(line);
            file_data currData;
            currData.parse_line(line);
            market_data.push_back(currData);
            toAdd= true;
        }
        else if(toAdd)
        {
            import_data(market_data, stock, (int)IMPORT_TYPE::ADDING);
            isStockHeader = true;
            toAdd = false;
            market_data.clear();
        }
    }
    // Für den letzten
    if (!market_data.empty())
        import_data(market_data, stock, static_cast<int>(IMPORT_TYPE::ADDING));
    if(!imported) std::cout << "Warning: Could not import data!" << std::endl;
}
