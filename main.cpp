#include <iostream>
#include "InputImplementation.h"
#include "clustering_vectors.h"
#include "clustering_curves.h"
#include "utilities.h"

//#define whichInitialization 2
//#define whichAssignment 1
//#define whichUpdate 2
int main(int argc, char **argv) {
    string inputFile, configurationFile, outputFile;
    int whichInitialization = 1, whichAssignment = 1, whichUpdate = 2;
    bool complete = false;
    assign_values_from_arguments(argc, argv, inputFile, configurationFile, complete, whichInitialization,
                                 whichAssignment, whichUpdate, outputFile);
    cout << inputFile << " " << configurationFile << " " << outputFile << " " << complete << " " << whichInitialization
         << " " << whichAssignment << " " << whichUpdate << endl;

    unsigned int k = 5;
    unsigned int L_grids = 2;
    unsigned int L_hashtables = 3;
    unsigned int k_of_lsh;
    assign_values_from_configurationFile(configurationFile, k, L_grids, k_of_lsh, L_hashtables);
    cout << k << " " << L_grids << " " << L_hashtables << " " << k_of_lsh << endl;
    ifstream file;
    file.open(inputFile);
    string firstLine;
    getline(file, firstLine);  //read first line
    file.close();

    if (firstLine == "vectors") {
        InputGenericVector<double> pointsVector(
                inputFile);//Implementation in InputImplementation.h/.cpp
        VectorClustering<double> vectorClustering(pointsVector, k, whichInitialization, whichAssignment, whichUpdate,
                                                  k_of_lsh, L_hashtables, complete);
    } else if (firstLine == "curves") {
        unsigned int maxCurveSize = 0, minCurveSize = 4294967295;
        InputGenericVector<pair<double, double>> curvesVector(inputFile,
                                                              maxCurveSize, minCurveSize, false);


        CurveClustering<pair<double, double>> curveClustering(curvesVector, k, maxCurveSize, minCurveSize,
                                                              whichInitialization, whichAssignment,
                                                              whichUpdate, k_of_lsh, L_grids, complete);
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