#include <iostream>
#include "InputImplementation.h"
#include "clustering_vectors.h"
#include "clustering_curves.h"
#include "Dtw.h"
#define whichInitialization 1
#define whichAssignment 2
#define whichUpdate 2
int main() {
    bool isVectors = true;
    unsigned int k = 5;
    if (isVectors) {
        InputGenericVector<int> pointsVector(
                "C:\\Users\\User\\Desktop\\query_small_id");//Implementation in InputImplementation.h/.cpp
        VectorClustering<int> vectorClustering(pointsVector, k, whichInitialization, whichAssignment, whichUpdate);
    } else {
        unsigned int maxCurveSize = 0, minCurveSize = 4294967295;
        InputGenericVector<pair<double, double>> curvesVector("C:\\Users\\User\\Desktop\\trajectories_dataset",
                                                              maxCurveSize, minCurveSize, false);


        CurveClustering<pair<double, double>> curveClustering(curvesVector, k, maxCurveSize, minCurveSize,
                                                              whichInitialization, whichAssignment,
                                                              whichUpdate);
//        vector<vector<Cell>> C;         //2-d vector
//        double dist = Dtw(curvesVector.itemValues[0].second, curvesVector.itemValues[1].second, C);
//        cout << "diist " << dist << endl;
//        cout << "Distance: "
//             << C[curvesVector.itemValues[0].second.size() - 1][curvesVector.itemValues[1].second.size() - 1].value
//             << endl;
//        vector<pair<int, int>> path = printPath(C);
//
//        for (auto &num : path) { //print path
//            cout << "(" << num.first << "," << num.second << ")" << "\t";
//        }
//        cout << endl;
    }
    return 5;
}