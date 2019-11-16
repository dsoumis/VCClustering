#ifndef CURVES_DTW_H
#define CURVES_DTW_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class Cell {
public:
    double value;
    Cell *previous;
    int i;  //for position in the array
    int j;

    Cell() {
        value = 0.0;
        previous = nullptr;
        i = 0;
        j = 0;
    }
};

vector<pair<int, int>> printPath(vector<vector<Cell>> &);

double calculateDistance(double, double, double, double);

pair<double, int> min(double, double, double);

template<class inputData>
double calculateC(vector<vector<Cell>> &, vector<inputData> const &, vector<inputData> const &);

template<class inputData>
double Dtw(vector<inputData> const &, vector<inputData> const &, vector<vector<Cell>> &);


#endif //CURVES_DTW_H