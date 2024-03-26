#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "Stock.h"
#include <string>


class HashTable
{
    public:
        HashTable(int size);
        virtual ~HashTable();
        int hash_function(Stock** arr, std::string key);
        int find_hash_index(Stock** arr, std::string key);
    private:
        int quadrating_probing(Stock** arr, int hashIndex);
        int get_ascii_sum(std::string characters);
        int size;
};

#endif // HASHTABLE_H
