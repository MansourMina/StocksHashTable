#include "Manager.h"
#include <iostream>

Manager::Manager()
{
    stocksCount= 0;
}

Manager::~Manager()
{
    for(int i = 0; i < stocksCount; i++)
        delete stocks[i];
}

void Manager::add_stock(std::string name, std::string wkn)
{

    std::string stockInitials = get_initials(stock_toupper(name));
    int hashIndex = hash_function(stockInitials);
    Stock* stock = new Stock(stock_toupper(name), stockInitials, stock_toupper(wkn));
    if(stocks[hashIndex] == nullptr){
        stocks[hashIndex] = stock;
        stock->next = nullptr;

        return;
    }
    Stock* currStock = stocks[hashIndex];
    while(currStock->next != nullptr){
        currStock = currStock->next;
    }
    currStock->next = stock;


}

std::string Manager::get_initials(std::string name)
{
    std::string initials;
    initials.resize(name.length()/2);
    for(int i = 0; i < name.length()/2; i++)
    {
        initials[i] = name[i];
    }
    return initials;
}


int Manager::hash_function(std::string initials)
{
    double asciiSum = 0;
    for(int i = 0; i < initials.length(); i++)
    {
        asciiSum += int(initials[i]);
    }
    return int(asciiSum) % STOCKS_SIZE;
}


Stock* Manager::find_stock(std::string name)
{
    name = stock_toupper(name);
    std::string initials = get_initials(name);
    int hashIndex = hash_function(initials);

    Stock* currStock = stocks[hashIndex];
    if(currStock == nullptr) return nullptr;
    while(currStock->get_name() != name && currStock != nullptr){
        currStock = currStock->next;
    }

    return currStock;
}

std::string Manager::stock_toupper(std::string characters)
{
    for(int i = 0; i < characters.length(); i++)
    {
        characters[i] = std::toupper(characters[i]);
    }
    return characters;
}
