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
#include "GridImplementation.h"
#include "HashPair.h"
#include "kmeansPlusPlus.h"

using namespace std;

template<class inputData>
class CurveClustering {
private:
    unsigned int k;
    //Centers consist of a string ItemID the vector of this item and its index in curvesVector. If the item is out of dataset ItemID="OutOfDataset" and index=-1
    vector<tuple<string, vector<inputData>, int>> centers;

    unsigned int maxCurveSize, minCurveSize;

    vector<struct cluster<inputData>> clusters;

    //Will be used to save the distances between vectors and save a lot of time when there is a demand of recalculating.
    //New hashing because unordered map can't support pairs
    //The key to unordered map are the indexes in curvesVector


    unordered_map<pair<int, int>, double, hash_pair> calculatedDistances;

    class LSH {
    public:
        int L_grid;
        int k_vec;
        double max_coord;
        double radius;
        vector<Grid<inputData>> grids;
        vector<HashTables<inputData>> hashTables;
        vector<HashFunctions<double>> hashFunctions;
        vector<double> w; //One w for each grid
        vector<vector<vector<double>>> concatenated_gridCurve_vectors;

        LSH(int const &k_vec_given, int const &L_given,
            double const &radius_given, unsigned int const &minCurveSize, unsigned int const &maxCurveSize,
            unsigned int const &dimensions, double const &delta, InputGenericVector<inputData> const &curvesVector,
            double const &max_coord_given) {
            k_vec = k_vec_given;
            L_grid = L_given;
            radius = radius_given;
            max_coord = max_coord_given;

            grids.reserve((unsigned long) L_grid);
            hashFunctions.reserve((unsigned long) L_grid);
            hashTables.reserve((unsigned long) L_grid);
            w.resize((unsigned long) L_grid);
            concatenated_gridCurve_vectors.resize((unsigned long) L_grid);
            for (unsigned int v = 0; v < L_grid; v++) {
                concatenated_gridCurve_vectors[v].resize(curvesVector.itemValues.size());
            }

            for (unsigned int i = 0; i < L_grid; i++) {
                grids.emplace_back(Grid<inputData>(minCurveSize, dimensions, delta));//Create L_grid grids

                //Create the concatenated grid curve vector for each curve of each grid
                for (unsigned int curve = 0; curve < curvesVector.itemValues.size(); curve++) {
                    concatenated_gridCurve_vectors[i][curve] = grids[i].concatenated_vector_from_gridCurve(
                            curvesVector.itemValues[curve].second, maxCurveSize, max_coord);
                }
                //Exact Neighbours here calculates only 5% of neighbors for speed
                ExactNeighboursVector<double> temp_concatenatedNeighbors(concatenated_gridCurve_vectors[i],
                                                                         concatenated_gridCurve_vectors[i], true);
                w[i] = temp_concatenatedNeighbors.wCalculator();

                hashFunctions.emplace_back(
                        HashFunctions<double>(k_vec, maxCurveSize,
                                              w[i]));//Create L_grid g functions. One for each grid.
                hashTables.emplace_back(HashTables<inputData>(1)); //Create L_grid LSHs with one hash table each.
            }
        }
    };

    LSH *lsh;

    //Unordered map of itemIDs
    //key is index in curvesVector
    //pair of index of cluster assigned to, distance from centroid of cluster
    unordered_map<int, pair<int, double>> assignedItems;

    double fast_distance_calculationC(int const &index1, int const &index2, vector<inputData> const &item1,
                                      vector<inputData> const &item2);

    void InitializationSimplest(InputGenericVector<inputData> const &curvesVector);

    void InitializationKmeansPlusPlus(InputGenericVector<inputData> const &curvesVector);

    void AssignmentSimplest(InputGenericVector<inputData> const &curvesVector);

    void
    ReverseAssignmentPreload(InputGenericVector<inputData> &curvesVector, int const &k_vec_given, int const &L_given,
                             unsigned int const &dimensions);

    void ReverseAssignment(InputGenericVector<inputData> const &curvesVector);


    void InitializationUpdate(unsigned int const &cluster_index, InputGenericVector<inputData> const &curvesVector,
                              unsigned long &lamda, vector<inputData> &C);

    void UpdateSimplest(InputGenericVector<inputData> const &curvesVector, bool &unchangedCenters);

    void UpdateALaLoyd(InputGenericVector<inputData> const &curvesVector, bool &unchangedCenters);

    void Silhouette(InputGenericVector<inputData> &curvesVector);

    void Printing(unsigned int const &whichInitialization, unsigned int const &whichAssignment,
                  unsigned int const &whichUpdate, bool const &complete, double const &duration,
                  InputGenericVector<inputData> &curvesVector);

public:
    explicit CurveClustering(InputGenericVector<inputData> &curvesVector, unsigned int const &k_given,
                             unsigned int const &maxSize, unsigned int const &minSize,
                             unsigned int const &whichInitialization, unsigned int const &whichAssignment,
                             unsigned int const &whichUpdate, unsigned int const &k_of_lsh, unsigned int const &L_grid,
                             bool const &complete);
};

#endif //SECOND_STEP_CLUSTERING_CURVES_H
