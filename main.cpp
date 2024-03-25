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
    std::cout << "ADD[1] PRINT[2] DELETE[2] IMPORT[3] SEARCH[4] PLOT[5] SAVE[6] LOAD[7] QUIT[8]: ";
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
        std::cout << "DELETE" << std::endl;
        break;
    }
    case IMPORT:
    {
        std::cout << "IMPORT" << std::endl;
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

        std::cout << "PLOT" << std::endl;
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
    {
        break;
    }
    default:
        break;
    }


}
