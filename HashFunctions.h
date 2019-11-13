#ifndef LSH_HASHFUNCTIONS_H
#define LSH_HASHFUNCTIONS_H

#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include "InputImplementation.h"

using namespace std;

template<class inputData>
class HashFunctions {
private:
    vector<vector<double>> s;
    vector<int> m;
    int M;

    void calculateSi(unsigned int const &d, int const &k, double const &w);

    void calculate_mi(unsigned int const &d);

    int calculate_ai(inputData const &x, double const &s, double const &w);

    int calculateH(vector<inputData> const &x, unsigned int whichHashFunction, double const &w, unsigned int const &k);

public:
    explicit HashFunctions(int k, unsigned long d, double const &w) {
        s.resize((unsigned long) k);
        calculateSi((unsigned int) d, k, w);
        M = (int) pow(2, 32 / k);
        calculate_mi((unsigned int) d);
    }

    unsigned int gCalculator(vector<inputData> const &x, unsigned int const &k, double const &w);
};

#endif //LSH_HASHFUNCTIONS_H
