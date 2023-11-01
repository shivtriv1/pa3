/*
 *  Separate chaining hashtable
 */

#ifndef __CHAINING_HASH_H
#define __CHAINING_HASH_H

 // Standard library includes
#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>

// Custom project includes
#include "Hash.h"

// Namespaces to include
using std::vector;
using std::list;
using std::pair;
using std::move;

using std::cout;
using std::endl;


//
// Separate chaining based hash table - derived from Hash
//
template<typename K, typename V>
class ChainingHash : public Hash<K, V> {
private:

    vector<list<pair<K, V>>> table;

    int total_size;

    int num_buckets;

    const float max_load_factor = 0.75;



public:

    ChainingHash(int n = 101): 
        table(n), total_size(0), num_buckets(n) 
    {}

    int size() override {
        return total_size;
    }

    V operator[](const K& key) override {

        int index = hash(key);

        for (const pair<K, V>& pair : table[index]) 
        {
            if (pair.first == key) 
            {
                return pair.second;
            }
        }

    }

    bool insert(const pair<K, V>& pair) override {
        
        int index = hash(pair.first);

        table[index].push_back(pair);
        total_size++;

        
        if (max_load_factor < load_factor())
        {
            rehash();
        }

        return true;
    }

    void erase(const K& key) override {

        int index = hash(key);

        for (auto i = table[index].begin(); i != table[index].end(); ++i) 
        {
            if (i->first == key) 
            {
                table[index].erase(i);
                total_size = total_size - 1;
                return;
            }
        }
    }

    void clear() override {

        table = vector<list<pair<K, V>>>(num_buckets);
        total_size = 0;
    }

    int bucket_count() override {
        return num_buckets;
    }

    float load_factor() override {
        return static_cast<float>(total_size) / num_buckets;
    }

private:


    void rehash() 
    {
    
        int temp_buckets = findNextPrime(num_buckets * 2);
        vector<list<pair<K, V>>> temp_table(temp_buckets);

        
        for (const auto& bucket : table) 
        {
            for (const auto& pair : bucket) 
            {
                int index = hash(pair.first);
                temp_table[index].push_back(pair);
            }
        }

        table = move(temp_table);
        num_buckets = temp_buckets;
    }

    int findNextPrime(int n) {

        while (!isPrime(n)) 
        {
            n++;
        }

        return n;
    }

    int isPrime(int n) {

        if (n <= 1) 
        {
            return false;
        }

        for (int i = 2; i <= sqrt(n); i++) 
        {
            if (n % i == 0) {
                return false;
            }
        }

        return true;
    }

    int hash(const K& key) {
        return key % num_buckets;
    }
};


#endif