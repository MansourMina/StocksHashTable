#include "HashTable.h"
#include <iostream>


HashTable::HashTable(int size)
{

    this->size = size;
}

HashTable::~HashTable()
{
    //dtor
}

int HashTable::quadrating_probing(Stock** arr, int hashIndex)
{
    int attempts = 1, currIndex = (hashIndex + 1) % size;
    while(arr[currIndex] != nullptr)
    {
        attempts++;
        currIndex = (hashIndex + (attempts * attempts) % size);
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
    int hashIndex = get_ascii_sum(key) % size;
    if(arr[hashIndex] == nullptr) return hashIndex;
    return quadrating_probing(arr, hashIndex);
}

int HashTable::find_hash_index(Stock** arr, std::string key)
{
    int hashIndex = get_ascii_sum(key) % size;
    if(arr[hashIndex] == nullptr) return -1;
    if(arr[hashIndex]->get_initials() == key || arr[hashIndex]->get_name() == key) return hashIndex;
    int attempts = 1, currIndex = (hashIndex + 1) % size;
    while(arr[hashIndex]->get_initials() == key || arr[hashIndex]->get_name() == key)
    {
        attempts++;
        currIndex = (hashIndex + (attempts * attempts) % size);
        if(currIndex > size) return -1;
    }
    return currIndex;
}
