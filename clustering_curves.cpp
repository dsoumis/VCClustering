#include "clustering_curves.h"

int generateNumberC(int const &range_from,
                    int const &range_to) {      //https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution

    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_int_distribution<> distr(range_from, range_to);

    return distr(generator);

}

template<class inputData>
void CurveClustering<inputData>::InitializationSimplest(InputGenericVector<inputData> const &pointsVector,
                                                        unsigned long const &lamda) {
    centers.resize(k);
    //We will use an unordered map so if a random value has been already selected to select another one
    unordered_map<int, bool> alreadySelectedRandom;
    for (unsigned int i = 0; i < k; ++i) {
        int randomIndex = generateNumberC(0, (int) pointsVector.itemValues.size() - 1);
        //While generated index has already been used, generate another one || curve size of this index is <lamda
        while (alreadySelectedRandom.find(randomIndex) != alreadySelectedRandom.end() ||
               pointsVector.itemValues[randomIndex].second.size() < lamda) {
            randomIndex = generateNumberC(0, (int) pointsVector.itemValues.size() - 1);
        }
        alreadySelectedRandom[randomIndex] = true;
        centers[i].first = pointsVector.itemValues[randomIndex].first; //Assign randomly the ItemID of a curve of pointVector

        //If curve's length is already lamda assign the whole sequence of coordinates
        if (pointsVector.itemValues[randomIndex].second.size() == lamda) {
            centers[i].second = pointsVector.itemValues[randomIndex].second; //Assign randomly the coordinates of a curve of pointVector
            //If curve's length is greater than lamda generate an index in sequence of coordinates
        } else {
            int randomSubsequenceIndex = generateNumberC(0,
                                                         (int) pointsVector.itemValues[randomIndex].second.size() - 1);
            //check if points after index are enough to successfully create the subsequence. index + point after it must equal lamda
            while (true) {
                unsigned int subsequence_lentgh = 0;
                for (unsigned long ind = randomSubsequenceIndex;
                     ind < pointsVector.itemValues[randomIndex].second.size(); ++ind) {
                    subsequence_lentgh++;
                }
                if (subsequence_lentgh == lamda) {
                    //Assign the lamda-subsequence
                    for (unsigned long ind = randomSubsequenceIndex;
                         ind < pointsVector.itemValues[randomIndex].second.size(); ++ind) {
                        centers[i].second.push_back(pointsVector.itemValues[randomIndex].second[ind]);
                    }
                    break;
                } else {
                    randomSubsequenceIndex = generateNumberC(0,
                                                             (int) pointsVector.itemValues[randomIndex].second.size() -
                                                             1);
                }
            }
        }
    }
}

template<class inputData>
void CurveClustering<inputData>::AssignmentSimplest(InputGenericVector<inputData> const &curvesVector) {

    //For each assignment erase the vector of clusters
    clusters.clear();
    clusters.shrink_to_fit();
    clusters.resize(k);
    vector<vector<Cell>> tempFoo;
    //Assignment:For each curve we find the nearest center using dtw metric
    for (unsigned long curve = 0; curve < curvesVector.itemValues.size(); ++curve) {
        double min = Dtw(curvesVector.itemValues[curve].second, centers[0].second, tempFoo);
        unsigned int minCentroidIndex = 0;
        for (unsigned int centroid = 1; centroid < k; ++centroid) {
            double distance_from_centroid = Dtw(curvesVector.itemValues[curve].second, centers[centroid].second,
                                                tempFoo);
            if (min > distance_from_centroid) {
                min = distance_from_centroid;
                minCentroidIndex = centroid;
            }
        }

        clusters[minCentroidIndex].centroid = centers[minCentroidIndex].first;
        clusters[minCentroidIndex].centroidCoordinates = centers[minCentroidIndex].second;
        clusters[minCentroidIndex].ItemIDs.push_back(curvesVector.itemValues[curve].first);
        clusters[minCentroidIndex].indexes.push_back(curve);
    }

}

template<class inputData>
CurveClustering<inputData>::CurveClustering(InputGenericVector<inputData> &pointsVector, unsigned int const &k_given,
                                            unsigned int const &whichInitialization,
                                            unsigned int const &whichAssignment,
                                            unsigned int const &whichUpdate) {
    k = k_given;
    unsigned long lamda = 0;
    for (auto const &curve:pointsVector.itemValues) {
        lamda += curve.second.size();
    }
    lamda /= pointsVector.itemValues.size();
    if (whichInitialization == 1)
        InitializationSimplest(pointsVector, lamda);
    else
        cout << "foo" << endl;

}

template
class CurveClustering<pair<double, double>>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.