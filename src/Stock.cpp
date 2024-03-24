#include "Stock.h"
#include <string>
#include <iostream>

Stock::Stock(std::string name, std::string initials, std::string wkn){
    this->name = name;
    this->initials= initials;
    this->wkn = wkn;
}

Stock::~Stock(){
}

std::string Stock::get_name(){
    return name;
}

std::string Stock::get_wkn(){
    return wkn;
}

std::string Stock::get_initials(){
    std::cout << initials << std::endl;
    return initials;
}


