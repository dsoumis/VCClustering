#ifndef LSH_HASHTABLES_H
#define LSH_HASHTABLES_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <cstdio>
#include <ctime>
#include <chrono>
#include "HashFunctions.h"

using namespace std;

template<class inputData>
class HashTables {
private:
    //A vector of hashtables. Unordered multimap allows many values to 1 key,which means collisions in 1 bucket.
    //A bucket has an unsigned int key and a tuple of itemID, points of item
    vector<unordered_multimap<unsigned int, tuple<string, vector<inputData>, int> > > hashTables;

    double manhattanDistance(vector<inputData> const &point, vector<inputData> const &query);

public:
    explicit HashTables(unsigned int L) {
        hashTables.resize((unsigned long) L);
    }

    void insertHashtable(unsigned int whichHashTable, unsigned int const &g, pair<string, vector<inputData>> &point,
                         int const &index);

    pair<vector<string>, vector<int>> rangeSearch(pair<string, vector<inputData >> &query, double radius,
                                                  vector<HashFunctions<int>> hashFunctions, unsigned int const &k,
                                                  double const &w);
};

#endif //LSH_HASHTABLES_H
