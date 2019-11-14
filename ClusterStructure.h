#ifndef SECOND_STEP_CLUSTERSTRUCTURE_H
#define SECOND_STEP_CLUSTERSTRUCTURE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;
struct cluster {
    string centroid;
    vector<int> centroidCoordinates;
    vector<string> ItemIDs;
    vector<int> indexes;
};
#endif //SECOND_STEP_CLUSTERSTRUCTURE_H
