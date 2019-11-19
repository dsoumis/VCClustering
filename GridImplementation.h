#ifndef LSH_CURVES_GRIDIMPLEMENTATION_H
#define LSH_CURVES_GRIDIMPLEMENTATION_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <random>
#include <algorithm>

using namespace std;

template<class inputData>
class Grid {
private:
    double delta;
    vector<double> t;

    void calculateTi(unsigned int const &dimensions);

    pair<double, double> gridCurvePointMaker(pair<double, double> const &point);

    double calculate_ai(double const &x, unsigned int const &whichT);

public:
    Grid(unsigned int const &min_m, unsigned int const &dimensions, double const &del) {
        delta = del;
        t.reserve(dimensions);
        calculateTi(dimensions);
    }

    vector<double>
    concatenated_vector_from_gridCurve(vector<inputData> const &curve, unsigned int const &max_vectorSize,
                                       double const &max_coord);
};

#endif //LSH_CURVES_GRIDIMPLEMENTATION_H
