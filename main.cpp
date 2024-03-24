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

void search_stock_input(int &searchType, std::string &searchName)
{

    std::cout << "Search by Name[1] Initials[2]: ";
    std::cin >> searchType;
    while(1)
    {
        switch(searchType)
        {
        case SEARCH_TYPE::BY_NAME:
            std::cout << "Name: ";
            break;
        case SEARCH_TYPE::BY_INITIALS:
            std::cout << "Initials: ";
            break;
        default:
            continue;
        }
        break;
    }

    std::cin >> searchName;
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
        std::string searchName;
        int searchType;
        search_stock_input(searchType, searchName);
        Stock* foundStock = manager->search_stock(searchType, searchName);
        std::cout << "Name: " << foundStock->get_name() << std::endl;
        std::cout << "Initials: " << foundStock->get_initials() << std::endl;
        std::cout << "WKN: " << foundStock->get_wkn() << std::endl;
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
