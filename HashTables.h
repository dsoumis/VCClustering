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
#include "ClusterStructure.h"

using namespace std;

template<class inputData>
class HashTables {
private:
    //A vector of hashtables. Unordered multimap allows many values to 1 key,which means collisions in 1 bucket.
    //A bucket has an unsigned int key and a tuple of itemID, points of item + an int index in initial vector + pair of index of cluster assigned to and distance from centroid of cluster
    vector<unordered_multimap<unsigned int, tuple<string, vector<inputData>, int> > > hashTables;

    double manhattanDistance(vector<inputData> const &point, vector<inputData> const &query);

public:
    explicit HashTables(unsigned int L) {
        hashTables.resize((unsigned long) L);
    }

    void insertHashtable(unsigned int whichHashTable, unsigned int const &g, pair<string, vector<inputData>> &point,
                         int const &index);

    void rangeSearch(pair<string, vector<inputData >> &query, double radius,
                     vector<HashFunctions<inputData>> hashFunctions, unsigned int const &k,
                     double const &w, vector<struct cluster<inputData>> &clusters, int const &index,
                     unordered_map<string, tuple<int, double, int>> &assignedItems);
};

#endif //LSH_HASHTABLES_H
