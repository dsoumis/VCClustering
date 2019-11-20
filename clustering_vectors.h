#ifndef SECOND_STEP_CLUSTERING_VECTORS_H
#define SECOND_STEP_CLUSTERING_VECTORS_H

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
using namespace std;

template<class inputData>
class VectorClustering {
private:
    unsigned int k;
    vector<pair<string, vector<inputData>>> centers;

    class LSH {
    public:
        int L;
        int k_vec;
        double w;
        double radius;
        HashTables<inputData> *hashTables;
        vector<HashFunctions<inputData>> hashFunctions;

        LSH(int const &k_vec_given, int const &L_given, double const &w_given, unsigned long const &size,
            double const &radius_given) {
            k_vec = k_vec_given;
            L = L_given;
            w = w_given;
            radius = radius_given;
            hashTables = new HashTables<inputData>((unsigned int) L);
            hashFunctions.reserve((unsigned long) L);
            for (unsigned int i = 0; i < L; i++) { //Create L g functions
                hashFunctions.emplace_back(HashFunctions<inputData>(k_vec, size, w));
            }
        }

        ~LSH() {
            free(hashTables);
        }
    };

    LSH *lsh;
    //Unordered map of itemIDs
    //key is index in curvesVector
    //pair of index of cluster assigned to, distance from centroid of cluster
    unordered_map<int, pair<int, double>> assignedItems;
    vector<struct cluster<inputData>> clusters;

    void InitializationSimplest(InputGenericVector<inputData> const &pointsVector);

    void AssignmentSimplest(InputGenericVector<inputData> const &pointsVector);

    void
    ReverseAssignmentPreload(InputGenericVector<inputData> &pointsVector, int const &k_vec_given, int const &L_given);

    void ReverseAssignment(InputGenericVector<inputData> const &pointsVector);

    void UpdateSimplest(InputGenericVector<inputData> const &pointsVector, bool &unchangedCenters);

    double manhattanDistance(vector<inputData> const &point, vector<inputData> const &query);

    void UpdateALaLoyd(InputGenericVector<inputData> const &pointsVector, bool &unchangedCenters);

public:
    explicit VectorClustering(InputGenericVector<inputData> &pointsVector, unsigned int const &k_given,
                              unsigned int const &whichInitialization, unsigned int const &whichAssignment,
                              unsigned int const &whichUpdate);
};

#endif //SECOND_STEP_CLUSTERING_VECTORS_H
