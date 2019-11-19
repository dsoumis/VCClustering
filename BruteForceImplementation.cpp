#include "BruteForceImplementation.h"
#include <cmath>

template<class inputData>
void ExactNeighboursVector<inputData>::save(string const &itemID, string const &neighborID, double const &value,
                                            double const &time) {
    neighboursVector.push_back(make_pair(itemID, make_tuple(neighborID, value, time)));
}


template<>
double ExactNeighboursVector<int>::manhattanDistance(vector<int> const &point, vector<int> const &query) {
    unsigned int distance = 0;
    for (unsigned long i = 0; i < point.size(); i++) {
        distance += abs(point[i] - query[i]);
    }
    return distance;
}

template<>
double ExactNeighboursVector<double>::manhattanDistance(vector<double> const &point, vector<double> const &query) {
    double distance = 0;
    for (unsigned int i = 0; i < point.size(); i++) {
        distance += abs(point[i] - query[i]);
    }
    return distance;
}

template<>
ExactNeighboursVector<double>::ExactNeighboursVector(vector<vector<double>> const &pointsVector,
                                                     vector<vector<double>> const &queriesVector,
                                                     bool input) { //Only 5% of neighbors

    for (unsigned int query = 0; query < queriesVector.size() * 0.05; query++) {
        double min = INT32_MAX;
        double neighborDistance = 0;
        for (unsigned int point = 0; point < pointsVector.size() * 0.05; point++) {
            if (point == query && input)
                continue;
            double distance = manhattanDistance(pointsVector[point], queriesVector[query]);

            //cout << "apostasi" << distance << endl;
            if (min > distance) {
                min = distance;
                neighborDistance = distance;
            }
        }
        this->save("temp_query", "temp", neighborDistance, 0);
    }
}
template<class inputData>
ExactNeighboursVector<inputData>::ExactNeighboursVector(InputGenericVector<inputData> const &pointsVector,
                                                        InputGenericVector<inputData> const &queriesVector,
                                                        bool input) {
    auto total_start = std::chrono::system_clock::now();
    for (unsigned long query = 0; query < queriesVector.itemValues.size(); query++) {
        auto start = std::chrono::system_clock::now();
        double min = INT32_MAX;
        string neighborID;
        double neighborDistance = 0;
        for (unsigned long point = 0; point < pointsVector.itemValues.size(); point++) {
            if (point == query && input)
                continue;
            double distance = manhattanDistance(pointsVector.itemValues[point].second,
                                                queriesVector.itemValues[query].second);

            //cout << "apostasi" << distance << endl;
            if (min > distance) {
                min = distance;
                neighborID = pointsVector.itemValues[point].first;
                neighborDistance = distance;
            }
        }
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> duration = end - start;
        this->save(queriesVector.itemValues[query].first, neighborID, neighborDistance, duration.count());
    }
    auto total_end = std::chrono::system_clock::now();
    std::chrono::duration<double> total_duration = total_end - total_start;
    cout << "Time for exact neighbors through brute force: " << total_duration.count() << endl;
}

template<class inputData>
ExactNeighboursVector<inputData>::ExactNeighboursVector(InputGenericVector<inputData> const &pointsVector,
                                                        InputGenericVector<inputData> const &queriesVector) {
    auto total_start = std::chrono::system_clock::now();
    for (unsigned long query = 0; query < queriesVector.itemValues.size(); query++) {
        auto start = std::chrono::system_clock::now();
        double min = INT32_MAX;
        string neighborID;
        double neighborDistance = 0;
        for (unsigned long point = 0; point < pointsVector.itemValues.size(); point++) {
            double distance = manhattanDistance(pointsVector.itemValues[point].second,
                                                queriesVector.itemValues[query].second);

            //cout << "apostasi" << distance << endl;
            if (min > distance) {
                min = distance;
                neighborID = pointsVector.itemValues[point].first;
                neighborDistance = distance;
            }
        }
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> duration = end - start;
        this->save(queriesVector.itemValues[query].first, neighborID, neighborDistance, duration.count());
    }
    auto total_end = std::chrono::system_clock::now();
    std::chrono::duration<double> total_duration = total_end - total_start;
    cout << "Time for exact neighbors through brute force: " << total_duration.count() << endl;
}

template<class inputData>
void ExactNeighboursVector<inputData>::printNeighborsToFile() {
    streambuf *psbuf, *backup;
    ofstream filestr;
    filestr.open("neighbors.txt");

    backup = cout.rdbuf();     // back up cout's streambuf

    psbuf = filestr.rdbuf();        // get file's streambuf
    cout.rdbuf(psbuf);         // assign streambuf to cout
    cout << neighboursVector.size() << endl;
    for (unsigned long i = 0; i < neighboursVector.size(); i++) {
        cout << "QueryID: " << neighboursVector[i].first << "    Neighbor: (Id: " << get<0>(neighboursVector[i].second)
             << ",Distance: " << get<1>(neighboursVector[i].second) << ",Time: " << get<2>(neighboursVector[i].second)
             << ")" << endl;
    }
    std::cout.rdbuf(backup);        // restore cout's original streambuf

    filestr.close();
}

template<class inputData>
double ExactNeighboursVector<inputData>::wCalculator() {
    double w = 0;
    for (unsigned long i = 0; i < neighboursVector.size(); i++) {
        w += get<1>(neighboursVector[i].second);
    }
    w = w / (neighboursVector.size());
    w = w * 4;
    return w;
}

template<class inputData>
double ExactNeighboursVector<inputData>::getRealDistance(int index) {
    return get<1>(neighboursVector[index].second);
}

template<class inputData>
double ExactNeighboursVector<inputData>::getRealTime(int index) {
    return get<2>(neighboursVector[index].second);
}

template
class InputGenericVector<int>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.
template
class ExactNeighboursVector<int>;

template
class InputGenericVector<double>;

template
class ExactNeighboursVector<double>;