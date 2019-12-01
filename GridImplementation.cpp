#include "GridImplementation.h"

double generateNumberGrid(double const &range_from,
                          double const &range_to) {      //https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution

    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_real_distribution<> distr(range_from, range_to);

    return distr(generator);

}

template<class inputData>
void Grid<inputData>::calculateTi(unsigned int const &dimensions) {
    for (int i = 0; i < dimensions; i++) {
        t.push_back(generateNumberGrid(0, delta));
    }
}

template<>
double Grid<pair<double, double>>::calculate_ai(double const &x, unsigned int const &whichT) {
    return round((x - t[whichT]) / delta);
}


template<>
pair<double, double> Grid<pair<double, double>>::gridCurvePointMaker(pair<double, double> const &point) {
    pair<double, double> value;
    //cout<<calculate_ai(point.first,0)<<" kai "<<calculate_ai(point.second,1)<<endl;
    value.first = calculate_ai(point.first, 0) * delta + t[0];
    value.second = calculate_ai(point.second, 1) * delta + t[1];
    return value;
}


template<>
vector<double> Grid<pair<double, double>>::concatenated_vector_from_gridCurve(vector<pair<double, double>> const &curve,
                                                                              unsigned int const &max_vectorSize,
                                                                              double const &max_coord) {
    vector<double> concatenatedVector;
    concatenatedVector.reserve(max_vectorSize); //Because each point is portrayed at 2D space
    pair<double, double> previousGridCurvePoint;
    for (unsigned long i = 0; i < curve.size(); i++) {
        pair<double, double> gridCurvePoint = gridCurvePointMaker(curve[i]);
        //cout<<"prwto "<<gridCurvePoint.first<<" deutero "<<gridCurvePoint.second<<endl;
        if (i > 0 && gridCurvePoint == previousGridCurvePoint)//remove consecutive duplicates
            continue;
        else
            previousGridCurvePoint = gridCurvePoint;

        //Save the coordinates as vector
        concatenatedVector.push_back(gridCurvePoint.first);
        concatenatedVector.push_back(gridCurvePoint.second);
    }
    //Fill the vector with zeroes so it matches with the biggest one in order to be used in LSH
    for (unsigned long i = concatenatedVector.size(); i < max_vectorSize; i++) {
        concatenatedVector.push_back(max_coord);
    }
    return concatenatedVector;
}

template
class Grid<pair<double, double>>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.