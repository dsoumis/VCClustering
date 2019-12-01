#ifndef SECOND_STEP_KMEANSPLUSPLUS_H
#define SECOND_STEP_KMEANSPLUSPLUS_H

#include <iostream>
#include "InputImplementation.h"
#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <cstdlib>
#include <cwctype>
#include "Dtw.h"

using namespace std;

template<class inputData>
void chooseRandomCenter(InputGenericVector<inputData> const &vector, ::vector<int> &centers);

template<class inputData>
void countDist(InputGenericVector<inputData> const &pointsVec, ::vector<int> &centers, ::vector<double> &D);

template<class inputData>
void createNewCenter(InputGenericVector<inputData> const &pointsVec, vector<int> &centers);

template<class inputData>
double manhattanDistance(::vector<inputData> const &center, ::vector<inputData> const &point);

template<class inputData>
pair<double, int> findNearestCenter(::vector<inputData> const &item, ::vector<int> &centers,
                                    InputGenericVector<inputData> const &pointsVec,
                                    unsigned long &i);

void calculatePartialSums(vector<double> &P, vector<double> &D);

double chooseRandomX(vector<double> &P);

int binarySearch(vector<double> &x, double my_number);

#endif //SECOND_STEP_KMEANSPLUSPLUS_H
