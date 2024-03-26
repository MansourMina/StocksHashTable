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

void Manager::del_map_initials(std::string key){

    (*initials).erase((*initials).find(key));
}

void Manager::del_map_name_initials(std::string key){

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
