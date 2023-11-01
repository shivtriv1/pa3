#ifndef __PROBING_HASH_H
#define __PROBING_HASH_H

#include <vector>
#include <stdexcept>
#include <cmath>
#include "Hash.h"

using std::vector;
using std::pair;
using std::move;

// Can be used for tracking lazy deletion for each element in your table
enum EntryState {
    EMPTY = 0,
    VALID = 1,
    DELETED = 2
};

template<typename K, typename V>
class ProbingHash : public Hash<K, V> {
private:

    vector<pair<K, V>> table;
    vector<EntryState> entryState;
    int total_size;
    int num_buckets;
    const float max_load_factor = 0.75;


public:
    ProbingHash(int n = 101) : 
        table(n), entryState(n, EMPTY), total_size(0), num_buckets(n) 
    {}

    ~ProbingHash() {
        this->clear();
        total_size = 0;
    }

    int size() override {
        return total_size;
    }

    
    
    V operator[](const K& key) override {
        
        int index = findIndex(key);
        if (index != -1) {
            return table[index].second;
        }
        else
        {
            return V();
        }
    }
    
    

    bool insert(const std::pair<K, V>& pair) override 
    {

        int index = findIndex(pair.first);
        
        if (index != -1) 
        {
            table[index] = pair;
            entryState[index] = VALID;
        }
        else 
        {
            index = findEmptyIndex(pair.first);

            if (index != -1) 
            {
                table[index] = pair;
                entryState[index] = VALID;
            }
            
        }

        total_size++;
        return true;
    }

    

    void erase(const K& key) override 
    {

        int index = findIndex(key);
        //int index =  hash(key);


        if (index != -1) {
            entryState[index] = DELETED;
            total_size--;
        }

    }

    void clear() override 
    {
       
        for (int i = 0; i < table.capacity(); i++) 
        {
            this->erase(i);
        }
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

        int temp_num_buckets = findNextPrime(num_buckets * 2);

        vector<pair<K, V>> temp_table(temp_num_buckets);
        vector<EntryState> temp_entryState(temp_num_buckets, EMPTY);

        for (int i = 0; i < num_buckets; i++) 
        {

            if (entryState[i] == VALID) 
            {

                int index = hash(table[i].first);
                int temp_index = index;
                int count = 0;

                while (temp_entryState[index] == VALID) 
                {
                    count++;
                    index = (temp_index + count) % temp_num_buckets;
                }

                temp_table[index] = table[i];
                temp_entryState[index] = VALID;
            }
        }

        table = move(temp_table);
        entryState = move(temp_entryState);
        num_buckets = temp_num_buckets;
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

    int findIndex(const K& key) 
    {

        int index = hash(key);
        int temp_index = index;
        int i = 0;
        do 
        {
            if (entryState[index] == VALID && table[index].first == key) 
            {
                return index;
            }

            i++;
            index = (temp_index + i) % num_buckets;

        } while (index != temp_index);

        return -1; 
    }

    int findEmptyIndex(const K& key) 
    {
        int index = hash(key);
        int temp_index = index;
        int i = 0;

        do {

            if (entryState[index] != VALID) 
            {
                return index;
            }

            i++;
            index = (temp_index + i) % num_buckets;

        } while (index != temp_index);

        return -1; 
    }
};

#endif //__PROBING_HASH_H
