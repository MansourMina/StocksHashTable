#include <iostream>
#include <string>
#include "Manager.h"
#include <limits>

enum MENU
{
    ADD=1,
    DELETE,
    IMPORT,
    SEARCH,
    PLOT,
    SAVE,
    LOAD,
    QUIT
};

bool menu_input(int &input);
void action(Manager* manager, int input);
void add_stock_input(std::string &name, std::string &wkn);

int main()
{
    Manager* manager = new Manager();
    std::cout << "MF Stock price management" << std::endl;
    int input;
    do
    {
        if(!menu_input(input)) continue;
        action(manager, input);
    }
    while(input != QUIT);
    delete manager;
    return 0;
}

bool valid_input(int input)
{

    switch(input)
    {
    case ADD:
    case DELETE:
    case IMPORT:
    case SEARCH:
    case PLOT:
    case SAVE:
    case LOAD:
    case QUIT:
        return true;
    default:
        return false;
    }
}

bool menu_input(int &input)
{
    std::cout << "ADD[1] DELETE[2] IMPORT[3] SEARCH[4] PLOT[5] SAVE[6] LOAD[7] QUIT[8]: ";
    if (!(std::cin >> input))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return valid_input(input);
}

void add_stock_input(Manager* manager, std::string &name, std::string &initials, std::string &wkn)
{
    while(1)
    {
        std::cout << "Enter stock name: ";
        std::cin >> name;
        if(name.length() < 5)
        {
            std::cout << "The stock name must be at least 5 characters long!" << std::endl;
            continue;
        }
        if(manager->name_exists(name))
        {
            std::cout << "Stock already exists! " << std::endl;
            continue;
        }
        break;
    }

    while(1)
    {
        std::cout << "Enter stock initials: ";
        std::cin >> initials;
        if(initials.length() < 3)
        {
            std::cout << "The stock initials must be at least 3 characters long!" << std::endl;
            continue;
        }
        if(manager->initials_exists(initials))
        {
            std::cout << "Initials already exists! " << std::endl;
            continue;
        }
        break;
    }


    do
    {
        std::cout << "Enter stock WKN: ";
        std::cin >> wkn;
        if(wkn.length() >= 10) break;
        std::cout << "The stock wkn must be at least 10 characters long!" << std::endl;
    }
    while(1);

}

void search_stock_input(std::string &searchName)
{
    do
    {
        std::cout << "Search: ";
        std::cin >> searchName;
        if(searchName.length() > 0) break;
    }
    while(1);
}

void del_stock_input(std::string &term)
{
    do
    {
        std::cout << "Stock: ";
        std::cin >> term;
        if(term.length() > 0) break;
    }
    while(1);
}

void data_stock_input(Manager* manager, std::string& file, std::string& stockName, int& type)
{
    while(1)
    {
        std::cout << "Stock: ";
        std::cin >> stockName;
        if(!manager->stock_exists(stockName))
        {
            std::cerr << "Failed: Stock not found!" << std::endl;
            continue;
        }
        break;
    }

    do
    {
        std::cout << "File: ";
        std::cin >> file;
        if(file.length() > 0) break;
    }
    while(1);
    Stock* stock = manager->search_stock(stockName);
    if(stock->get_market_data_count() != 0)
    {
        do
        {
            std::cout << "Replacing[1] or Adding[2] data: ";
            if (!(std::cin >> type))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            if(type >= 1 && type <= 2) break;
        }
        while(1);
    }

}

void action(Manager* manager, int input)
{
    switch(input)
    {
    case ADD:
    {
        std::string name,initials, wkn;
        add_stock_input(manager, name,initials, wkn);
        manager->add_stock(name,initials, wkn);
        break;
    }
    case DELETE:
    {
        std::string term;
        del_stock_input(term);
        manager->del_stock(term);
        break;
    }
    case IMPORT:
    {
        std::string file, stockName;
        int type;
        data_stock_input(manager, file, stockName, type);
        manager->add_market_data(file, stockName, type);
        break;
    }
    case SEARCH:
    {
        std::string searchName;
        search_stock_input(searchName);
        Stock* foundStock = manager->search_stock(searchName);
        if(foundStock == nullptr) std::cout << "Stock not found!" << std::endl;
        else manager->print_stock(foundStock);
        break;
    }
    case PLOT:
    {
        std::string searchName;
        search_stock_input(searchName);
        Stock* foundStock = manager->search_stock(searchName);
        if(foundStock == nullptr) std::cout << "Stock not found!" << std::endl;
        else manager->plot_market_data(foundStock);
        break;
    }
    case SAVE:
    {
        std::cout << "SAVE" << std::endl;
        break;
    }
    case LOAD:
    {
        std::cout << "LOAD" << std::endl;
        break;
    }
    case QUIT:
    default:
        break;
    }


}
