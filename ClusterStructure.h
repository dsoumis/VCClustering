#ifndef SECOND_STEP_CLUSTERSTRUCTURE_H
#define SECOND_STEP_CLUSTERSTRUCTURE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;
template<class inputData>
struct cluster {
    string centroid;
    vector<inputData> centroidCoordinates;
    vector<string> ItemIDs;
    vector<int> indexes;
};
#endif //SECOND_STEP_CLUSTERSTRUCTURE_H
