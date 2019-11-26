#ifndef SECOND_STEP_INITIALIZATION_H
#define SECOND_STEP_INITIALIZATION_H
#include <iostream>
#include "InputImplementatition.h"
#include <string>
#include <vector>
using namespace std;

template <class inputData>
void chooseRandomCenter(InputGenericVector<inputData>& vector, ::vector<int> &centers);
template <class inputData>
void countDist(InputGenericVector<inputData >& vector,::vector<int> &centers,::vector<double> &D);
template <class inputData>
void createNewCenter(InputGenericVector<inputData >& pointsVec, vector<int> &centers);
template <class inputData>
double manhattanDistance(::vector<inputData>& item, ::vector<inputData>& point);
pair<double,int> findNearestCenter(::vector<double>& item,::vector<int> &centers,InputGenericVector<double> &pointsVec,int &i  );
void calculatePartialSums(vector<double> &P, vector<double> &D);
double chooseRandomX(vector<double> &P);
int binarySearch(vector<double> &x, double my_number);
#endif //SECOND_STEP_INITIALIZATION_H
