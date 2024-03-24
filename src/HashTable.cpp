#include "HashTable.h"
#include <iostream>


HashTable::HashTable(int tSize)
{

    tableSize = tSize;
}

HashTable::~HashTable()
{
    //dtor
}

int HashTable::quadrating_probing(Stock** arr, int hashIndex)
{
    int attempts = 1, currIndex = hashIndex + 1;
    while(arr[currIndex] != nullptr)
    {
        attempts++;
        currIndex = hashIndex + (attempts * attempts);
    }
    return currIndex;
}

int HashTable::get_ascii_sum(std::string characters)
{
    int asciiSum = 0;
    for(int i = 0; i < characters.length(); i++)
        asciiSum += int(characters[i]);
    return asciiSum;
}

int HashTable::hash_function(Stock** arr, std::string key)
{
    int hashIndex = get_ascii_sum(key) % tableSize;
    if(arr[hashIndex] == nullptr) return hashIndex;
    return quadrating_probing(arr, hashIndex);
}

Stock* HashTable::find_obj(Stock** arr, std::string key)
{
    int hashIndex = get_ascii_sum(key) % tableSize;
    if(arr[hashIndex]->get_initials() == key || arr[hashIndex]->get_name() == key) return arr[hashIndex];
    int attempts = 1, currIndex = hashIndex + 1;
    while(arr[currIndex]->get_initials() != key )
    {
        attempts++;
        currIndex = hashIndex + (attempts * attempts);
        if(currIndex > tableSize) return nullptr;
    }
    return arr[currIndex];
}
