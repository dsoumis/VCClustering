#include "k_means_vectors.h"

int generateNumber(int const &range_from,
                   int const &range_to) {      //https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution

    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_int_distribution<> distr(range_from, range_to);

    return distr(generator);

}

template<class inputData>
inline double
VectorKMEANS<inputData>::manhattanDistance(vector<inputData> const &point, vector<inputData> const &query) {
    double distance = 0;
    for (unsigned int i = 0; i < point.size(); ++i) {
        distance += abs(point[i] - query[i]);
    }
    return distance;
}

template<class inputData>
void VectorKMEANS<inputData>::Initialization(InputGenericVector<int> const &pointsVector) {
    centers.resize(k);
    for (unsigned int i = 0; i < k; ++i) {
        int randomIndex = generateNumber(0, (int) pointsVector.itemValues.size() - 1);
        centers[i].first = pointsVector.itemValues[randomIndex].first; //Assign randomly the ItemID of a point of pointVector
        centers[i].second = pointsVector.itemValues[randomIndex].second; //Assign randomly the coordinates of a point of pointVector
    }
}

template<class inputData>
void VectorKMEANS<inputData>::Assignment(InputGenericVector<int> const &pointsVector) {
    bool unchangedCenters = false;
    while (!unchangedCenters) { //If the centers do not change we are done.
        //For each loop erase the vector of clusters
        clusters.clear();
        clusters.shrink_to_fit();
        clusters.resize(k);
        //Assignment:For each point we find the nearest center using l1 metric(manhattan distance)
        for (unsigned int point = 0; point < pointsVector.itemValues.size(); ++point) {
            double min = manhattanDistance(pointsVector.itemValues[point].second, centers[0].second);
            int minCentroidIndex = 0;
            for (unsigned int centroid = 1; centroid < k; ++centroid) {
                double distance_from_centroid = manhattanDistance(pointsVector.itemValues[point].second,
                                                                  centers[centroid].second);
                if (min > distance_from_centroid) {
                    min = distance_from_centroid;
                    minCentroidIndex = centroid;
                }
            }

            clusters[minCentroidIndex].centroid = centers[minCentroidIndex].first;
            clusters[minCentroidIndex].centroidCoordinates = centers[minCentroidIndex].second;
            clusters[minCentroidIndex].ItemIDs.push_back(pointsVector.itemValues[point].first);
            clusters[minCentroidIndex].indexes.push_back(point);
        }
        unchangedCenters = true;
        Update(pointsVector, unchangedCenters);
    }
}

template<class inputData>
void VectorKMEANS<inputData>::Update(InputGenericVector<int> const &pointsVector, bool &unchangedCenters) {
    for (unsigned int i = 0; i < k; ++i) {
        vector<int> temp;
        temp.resize(pointsVector.itemValues[0].second.size());
        //Calculate the mean of each dimension
        for (unsigned int j = 0; j < temp.size(); ++j) {
            for (auto index:clusters[i].indexes) {
                temp[j] += pointsVector.itemValues[index].second[j];
            }
            temp[j] = temp[j] /
                      (int) clusters[i].indexes.size(); //Pithano segmentation fault an to iosto cluster den exei kanena stoixeio
        }
        if (temp != centers[i].second) {
            unchangedCenters = false;
            centers[i].first = "OutofDataset";
            centers[i].second = temp;
        }
    }
}

template<class inputData>
VectorKMEANS<inputData>::VectorKMEANS(InputGenericVector<int> const &pointsVector, unsigned int k_given) {
    k = k_given;
    Initialization(pointsVector);
    Assignment(pointsVector);

    for (unsigned int i = 0; i < k; ++i) {
        cout << "CLUSTER-" << i + 1 << clusters[i].centroid << " {";
//            for(unsigned int item=0; item<clusters[i].centroidCoordinates.size(); ++item){
//                cout<<clusters[i].centroidCoordinates[item]<<" ";
//            }
//            cout<<endl;
//            cout<<"Items"<<endl;
        for (unsigned int item = 0; item < clusters[i].ItemIDs.size(); ++item) {
            if (item == clusters[i].ItemIDs.size() - 1) {
                cout << clusters[i].ItemIDs[item] << "}" << endl;
            } else
                cout << clusters[i].ItemIDs[item] << ", ";
        }
//            cout<<"Indexes"<<endl;
//            for(unsigned int item=0; item<clusters[i].indexes.size(); ++item){
//                cout<<clusters[i].indexes[item]<<" ";
//            }
//            cout<<endl;
    }

}

template
class VectorKMEANS<int>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.