#include <iostream>
#include <random>
#include "InputImplementation.h"

int generateNumber(int const &range_from,
                   int const &range_to) {      //https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution

    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_int_distribution<> distr(range_from, range_to);

    return distr(generator);

}
int main() {
    bool isVectors = true;
    unsigned int k = 5;
    if (isVectors) {
        InputGenericVector<int> pointsVector("C:\\Users\\User\\Desktop\\query_small_id");
        vector<int> randomCenters;
        randomCenters.resize(k);
        for (unsigned int i = 0; i < k; i++) {
            randomCenters[i] = generateNumber(0, (int) pointsVector.itemValues.size());
            cout << randomCenters[i] << endl;
        }
    }
}