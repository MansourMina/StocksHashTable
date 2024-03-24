#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "Stock.h"
#include <string>


class HashTable
{
    public:
        HashTable(int tSize);
        virtual ~HashTable();
        int hash_function(Stock** arr, std::string key);
        Stock* find_obj(Stock** arr, std::string key);
    private:
        int quadrating_probing(Stock** arr, int hashIndex);
        int get_ascii_sum(std::string characters);
        int tableSize;
};

#endif // HASHTABLE_H
