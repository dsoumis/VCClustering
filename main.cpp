#include <iostream>
#include "InputImplementation.h"
#include "k_means_vectors.h"

#define whichInitialization 1
#define whichAssignment 1
#define whichUpdate 1
int main() {
    bool isVectors = true;
    unsigned int k = 5;
    if (isVectors) {
        InputGenericVector<int> pointsVector(
                "C:\\Users\\User\\Desktop\\query_small_id");//Implementation in InputImplementation.h/.cpp
        VectorKMEANS<int> vectorKMEANS(pointsVector, k, whichInitialization, whichAssignment, whichUpdate);
    }
    return 5;
}