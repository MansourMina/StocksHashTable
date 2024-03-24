#include <iostream>
#include <string>
#include "Manager.h"

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

void menu_input(int &input);
void action(Manager* manager, int input);
void add_stock_input(std::string &name, std::string &wkn);

int main()
{
    Manager* manager = new Manager();
    std::cout << "MF Stock price management" << std::endl;
    int input;
    do
    {
        menu_input(input);
        action(manager, input);
    }
    while(input != QUIT);
    delete manager;
    return 0;
}

void menu_input(int &input)
{
    std::cout << "ADD[1] PRINT[2] DELETE[2] IMPORT[3] SEARCH[4] PLOT[5] SAVE[6] LOAD[7] QUIT[8]: ";
    std::cin >> input;
}

void add_stock_input(std::string &name, std::string &wkn)
{
    std::cout << "Enter stock name: ";
    std::cin >> name;
    std::cout << "Enter stock WKN: ";
    std::cin >> wkn;
}

void action(Manager* manager, int input)
{
    switch(input)
    {
    case ADD:
    {
        std::string name, wkn;
        add_stock_input(name, wkn);
        manager->add_stock(name, wkn);
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
        std::string name;
        std::cout << "Name: ";
        std::cin >> name;
        Stock* stock = manager->find_stock(name);
        std::cout << (stock != nullptr ? stock->get_initials():"Not Found!!") << std::endl;
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
        std::cout << "QUIT" << std::endl;
        break;
    }
    default:
        break;
    }
}
