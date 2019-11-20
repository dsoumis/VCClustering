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
#include "HashPair.h"

using namespace std;

template<class inputData>
class HashTables {
private:
    //A vector of hashtables. Unordered multimap allows many values to 1 key,which means collisions in 1 bucket.
    //A bucket has an unsigned int key and  an int index in initial vector
    vector<unordered_multimap<unsigned int, int> > hashTables;

    double manhattanDistance(vector<inputData> const &point, vector<inputData> const &query);

public:
    explicit HashTables(unsigned int L) {
        hashTables.resize((unsigned long) L);
    }

    void insertHashtable(unsigned int whichHashTable, unsigned int const &g, int const &index);

    void rangeSearch(tuple<string, vector<inputData>, int> &query, double radius,
                     vector<HashFunctions<inputData>> &hashFunctions, unsigned int const &k,
                     double const &w, vector<struct cluster<inputData>> &clusters, int const &index,
                     unordered_map<int, pair<int, double>> &assignedItems,
                     InputGenericVector<inputData> const &pointsVector,
                     unordered_map<pair<int, int>, double, hash_pair> &calculatedDistances);

    void rangeSearch(tuple<string, vector<pair<double, double>>, int> &query, double radius, unsigned int const &g,
                     vector<struct cluster<pair<double, double>>> &clusters, int const &index,
                     unordered_map<int, pair<int, double>> &assignedItems,
                     InputGenericVector<pair<double, double>> const &curvesVector,
                     unordered_map<pair<int, int>, double, hash_pair> &calculatedDistances);
};

#endif //LSH_HASHTABLES_H
