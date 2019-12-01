#ifndef SECOND_STEP_INITIALIZATIONCURVES_H
#define SECOND_STEP_INITIALIZATIONCURVES_H
#include <iostream>
#include <string>
#include <vector>
#include "InputImplementatition.h"
#include "Dtw.h"

using namespace std;

template <class inputData>
void chooseRandomCenter(InputGenericVector<inputData>& vector, ::vector<int> &centers);
template <class inputData>
void countDist(InputGenericVector<inputData >& vector,::vector<int> &centers,::vector<double> &D);
template <class inputData>
void createNewCenter(InputGenericVector<inputData >& curvesVec, vector<int> &centers);

pair<double,int> findNearestCenter(::vector<pair<double,double>>& item,::vector<int> &centers,InputGenericVector<pair<double,double>> &curvesVec,int &i  );
void calculatePartialSums(vector<double> &P, vector<double> &D);
double chooseRandomX(vector<double> &P);
int binarySearch(vector<double> &x, double my_number);
#endif //SECOND_STEP_INITIALIZATIONCURVES_H
