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
#include "HashTables.h"
#include "BruteForceImplementation.h"
#include "ClusterStructure.h"
using namespace std;

template<class inputData>
class VectorKMEANS {
private:
    unsigned int k;
    vector<pair<string, vector<inputData>>> centers;

    class LSH {
    public:
        int L;
        int k_vec;
        double w;
        double radius;
        HashTables<int> *hashTables;
        vector<HashFunctions<int>> hashFunctions;

        LSH(int const &k_vec_given, int const &L_given, double const &w_given, unsigned long const &size,
            double const &radius_given) {
            k_vec = k_vec_given;
            L = L_given;
            w = w_given;
            radius = radius_given;
            hashTables = new HashTables<int>((unsigned int) L);
            hashFunctions.reserve((unsigned long) L);
            for (unsigned int i = 0; i < L; i++) { //Create L g functions
                hashFunctions.emplace_back(HashFunctions<int>(k_vec, size, w));
            }
        }

        ~LSH() {
            free(hashTables);
        }
    };

    LSH *lsh;
    //Unordered map of itemIDs
    //tuple of index of cluster assigned to, distance from centroid of cluster and index in pointsVector
    unordered_map<string, tuple<int, double, int>> assignedItems;
    vector<struct cluster> clusters;

    void InitializationSimplest(InputGenericVector<int> const &pointsVector);

    void AssignmentSimplest(InputGenericVector<int> const &pointsVector);

    void ReverseAssignmentPreload(InputGenericVector<int> &pointsVector, int const &k_vec_given, int const &L_given);

    void ReverseAssignment(InputGenericVector<int> const &pointsVector);

    void UpdateSimplest(InputGenericVector<int> const &pointsVector, bool &unchangedCenters);

    double manhattanDistance(vector<inputData> const &point, vector<inputData> const &query);

    void UpdateALaLoyd(InputGenericVector<int> const &pointsVector, bool &unchangedCenters);

public:
    explicit VectorKMEANS(InputGenericVector<int> &pointsVector, unsigned int const &k_given,
                          unsigned int const &whichInitialization, unsigned int const &whichAssignment,
                          unsigned int const &whichUpdate);
};

#endif //SECOND_STEP_K_MEANS_VECTORS_H
