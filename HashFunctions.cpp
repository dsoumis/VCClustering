#include "HashFunctions.h"


double generateNumber(double const &range_from,
                      double const &range_to) {      //https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution

    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_real_distribution<> distr(range_from, range_to);

    return distr(generator);

}

template<class inputData>
void HashFunctions<inputData>::calculateSi(unsigned int const &d, int const &k, double const &w) {
    for (int i = 0; i < k; i++) {
        s[i].resize(d);
        for (unsigned int j = 0; j < d; j++) {
            s[i][j] = generateNumber(0, w);
        }
    }
}


template<class inputData>
inline int HashFunctions<inputData>::calculate_ai(inputData const &x, double const &S, double const &w) {
    int a = (int) floor((x - S) / w); //floors down the integer
    return a;
}

int mod(int a, int b) {
    return (a % b + b) % b;
}

int modular_power(int x, unsigned int y, int p) {
    int res = 1;      // Initialize result

    x = x % p;  // Update x if it is more than or
    // equal to p

    while (y > 0) {
        // If y is odd, multiply x with result
        if (y & 1u)
            res = (res * x) % p;

        // y must be even now
        y = y >> 1u; // y = y/2
        x = (x * x) % p;
    }
    return res;
}

template<class inputData>
void HashFunctions<inputData>::calculate_mi(unsigned int const &d) {
    m.resize(d);
    m[0] = 1;
    m[1] = modular_power(2, 32, M) - 5;
    //int value=(int)(((long)pow(2, 32)-5)%M);
    for (unsigned int i = 2; i < d; i++) {
        m[i] = modular_power(modular_power(2, 32, M) - 5, i, M);//(m[i-1]*value)%M;
    }
}

template<class inputData>
inline int
HashFunctions<inputData>::calculateH(vector<inputData> const &x, unsigned int whichHashFunction, double const &w,
                                     unsigned int const &k) {
    auto d = (unsigned int) x.size();
    int h = 0;
    int power = (int) d - 1;
    for (unsigned int i = 0; i < d; i++) {
        h += calculate_ai(x.at(i), s[whichHashFunction].at(i), w) * m[power];
        power--;
    }
    return mod(h, M);
}

template<class inputData>
inline unsigned int
HashFunctions<inputData>::gCalculator(const vector<inputData> &x, unsigned int const &k, double const &w) {
    unsigned int g = 0;
    for (unsigned int i = 0; i < k; i++) {
        int h = calculateH(x, i, w, k);
        g += h;
        g <<= 32 / k;
    }
    return g;
}

template
class HashFunctions<int>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.
template
class HashFunctions<double>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.