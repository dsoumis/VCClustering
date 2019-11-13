#ifndef SECOND_STEP_K_MEANS_VECTORS_H
#define SECOND_STEP_K_MEANS_VECTORS_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <random>
#include "InputImplementation.h"

using namespace std;

template<class inputData>
class VectorKMEANS {
private:
    unsigned int k;
    vector<pair<string, vector<inputData>>> centers;
    struct cluster {
        string centroid;
        vector<int> centroidCoordinates;
        vector<string> ItemIDs;
        vector<int> indexes;
    };
    vector<struct cluster> clusters;

    void Initialization(InputGenericVector<int> const &pointsVector);

    void Assignment(InputGenericVector<int> const &pointsVector);

    void Update(InputGenericVector<int> const &pointsVector, bool &unchangedCenters);

    double manhattanDistance(vector<inputData> const &point, vector<inputData> const &query);

public:
    explicit VectorKMEANS(InputGenericVector<int> const &pointsVector, unsigned int k_given);
};

#endif //SECOND_STEP_K_MEANS_VECTORS_H
