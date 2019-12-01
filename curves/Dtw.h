#ifndef SECOND_STEP_DTW_H
#define SECOND_STEP_DTW_H
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
double calculateDistance(double, double, double, double);
double min(double ,double ,double );
template <class inputData> double calculateC( vector<vector<double>> &, vector<inputData> &, vector<inputData> &);
template <class inputData> double Dtw(vector<inputData> &,vector<inputData> &);
#endif //SECOND_STEP_DTW_H
