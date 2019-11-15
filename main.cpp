#include <iostream>
#include "InputImplementation.h"
#include "clustering_vectors.h"
#include "clustering_curves.h"
#define whichInitialization 1
#define whichAssignment 1
#define whichUpdate 1
int main() {
    bool isVectors = false;
    unsigned int k = 5;
    if (isVectors) {
        InputGenericVector<int> pointsVector(
                "C:\\Users\\User\\Desktop\\query_small_id");//Implementation in InputImplementation.h/.cpp
        VectorClustering<int> vectorClustering(pointsVector, k, whichInitialization, whichAssignment, whichUpdate);
    } else {
        unsigned int maxCurveSize = 0, minCurveSize = 4294967295;
        InputGenericVector<pair<double, double>> pointsVector("C:\\Users\\User\\Desktop\\trajectories_dataset",
                                                              maxCurveSize, minCurveSize, false);

        CurveClustering<pair<double, double>> curveClustering(pointsVector, k, whichInitialization, whichAssignment,
                                                              whichUpdate);
    }
    return 5;
}