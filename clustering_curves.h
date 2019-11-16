#ifndef SECOND_STEP_CLUSTERING_CURVES_H
#define SECOND_STEP_CLUSTERING_CURVES_H

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
#include "HashTables.h"
#include "BruteForceImplementation.h"
#include "ClusterStructure.h"
#include "Dtw.h"

using namespace std;

template<class inputData>
class CurveClustering {
private:
    unsigned int k;
    vector<pair<string, vector<inputData>>> centers;
    vector<struct cluster<inputData>> clusters;
    void InitializationSimplest(InputGenericVector<inputData> const &pointsVector, unsigned long const &lamda);

    void AssignmentSimplest(InputGenericVector<inputData> const &pointsVector);

public:
    explicit CurveClustering(InputGenericVector<inputData> &curvesVector, unsigned int const &k_given,
                             unsigned int const &whichInitialization, unsigned int const &whichAssignment,
                             unsigned int const &whichUpdate);
};

#endif //SECOND_STEP_CLUSTERING_CURVES_H
