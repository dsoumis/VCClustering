#include "HashTables.h"

template<class inputData>
void HashTables<inputData>::insertHashtable(unsigned int whichHashTable, unsigned int const &g,
                                            pair<string, vector<inputData >> &point, int const &index) {
    hashTables[whichHashTable].insert(make_pair(g, make_tuple(point.first, point.second, index)));
}

template<class inputData>
inline double HashTables<inputData>::manhattanDistance(vector<inputData> const &point, vector<inputData> const &query) {
    double distance = 0;
    for (unsigned int i = 0; i < point.size(); i++) {
        distance += abs(point[i] - query[i]);
    }
    return distance;
}


template<class inputData>
pair<vector<string>, vector<int>>
HashTables<inputData>::rangeSearch(pair<string, vector<inputData >> &query, double radius,
                                   vector<HashFunctions<int>> hashFunctions, unsigned int const &k, double const &w) {
    vector<string> b_string; //Range Neighbors
    vector<int> b_indexes; //Range Neighbors
    bool allBucketsEmpty = true;
    for (unsigned int i = 0; i < hashTables.size(); i++) { //for i from 1 to L do
        unsigned int g = hashFunctions[i].gCalculator(query.second, k, w);
        for (auto p = hashTables[i].equal_range(g).first;
             p != hashTables[i].equal_range(g).second; p++) { //for each item p in bucket gi(q) do
            if (hashTables[i].count(g) >
                100 * hashTables.size()) //if large number of retrieved items (e.g.>10L) then Break
                break;

            allBucketsEmpty = false;
            //if dist(q,p) is within radius save the range neighbor
            double dist = manhattanDistance(get<1>(p->second), query.second);
            if (dist <= radius) {
                b_string.push_back(get<0>(p->second));
                b_indexes.push_back(get<2>(p->second));
            }

        }
    }
    if (allBucketsEmpty) {
        b_string.emplace_back("None");
        b_indexes.push_back(-1);
    }
    return make_pair(b_string, b_indexes);
}

template
class HashTables<int>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.