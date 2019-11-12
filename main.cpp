#include <iostream>
#include <random>
#include "InputImplementation.h"

inline int generateNumber(int const &range_from,
                          int const &range_to) {      //https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution

    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_int_distribution<> distr(range_from, range_to);

    return distr(generator);

}

template<class inputData>
inline double manhattanDistance(vector<inputData> const &point, vector<inputData> const &query) {
    double distance = 0;
    for (unsigned int i = 0; i < point.size(); ++i) {
        distance += abs(point[i] - query[i]);
    }
    return distance;
}
int main() {
    bool isVectors = true;
    unsigned int k = 5;
    if (isVectors) {
        InputGenericVector<int> pointsVector("C:\\Users\\User\\Desktop\\query_small_id");


        //Initialize k centers randomly
        vector<int> randomCenters;
        randomCenters.resize(k);
        for (unsigned int i = 0; i < k; ++i) {
            randomCenters[i] = generateNumber(0,
                                              (int) pointsVector.itemValues.size()); //random number between 0 and size of vector
            cout << randomCenters[i] << endl;
        }

        //We will use the values of vector randomCenters as index in pointsVector.itemValues
        for (unsigned int point = 0; point < pointsVector.itemValues.size(); ++point) {

            double min = manhattanDistance(pointsVector.itemValues[point].second,
                                           pointsVector.itemValues[randomCenters[0]].second);
            int minCentroid = randomCenters[0];
            for (unsigned int centroid = 1; centroid < k; ++centroid) {
                double distance_from_centroid = manhattanDistance(pointsVector.itemValues[point].second,
                                                                  pointsVector.itemValues[randomCenters[centroid]].second);
                if (min > distance_from_centroid) {
                    min = distance_from_centroid;
                    minCentroid = randomCenters[centroid];
                }
            }

        }

    }
}