#include "Manager.h"
#include <iostream>
#include <cmath>


Manager::Manager()
{
    stocksCount= 0;
    hashtable = new HashTable(STOCKS_SIZE);
}

Manager::~Manager()
{
    for(int i = 0; i < stocksCount; i++)
        delete stocks[i];
}

void Manager::add_stock(std::string name, std::string wkn)
{
    name = stock_toupper(name);
    std::string initials = get_initials(name);
    int hashIndex = hashtable->hash_function(stocks, initials);
    stocks[hashIndex] = new Stock(name, initials, wkn);
}

Stock* Manager::search_stock(int searchType, std::string searchKey){
    if((SEARCH_TYPE)searchType == BY_NAME) searchKey = get_initials(searchKey);
    return hashtable->find_obj(stocks, searchKey);
}

std::string Manager::get_initials(std::string name)
{
    char initials[5];
    if(name.length() > 3)
    {
        float position = 0;
        for(int i = 0; i < std::sqrt(name.length()); i++)
        {
            initials[i] = name[name.length() * position];
            position += 0.25;
        }

        return initials;
    }
    return name;
}



std::string Manager::stock_toupper(std::string characters)
{
    for(int i = 0; i < characters.length(); i++)
        characters[i] = std::toupper(characters[i]);

    return characters;
}
