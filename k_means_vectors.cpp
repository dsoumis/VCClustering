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
    for (unsigned long i = 0; i < point.size(); ++i) {
        distance += abs(point[i] - query[i]);
    }
    return distance;
}

template<class inputData>
void VectorKMEANS<inputData>::InitializationSimplest(InputGenericVector<int> const &pointsVector) {
    centers.resize(k);
    //We will use an unordered map so if a random value has been already selected to select another one
    unordered_map<int, bool> alreadySelectedRandom;
    for (unsigned int i = 0; i < k; ++i) {
        int randomIndex = generateNumber(0, (int) pointsVector.itemValues.size() - 1);
        while (alreadySelectedRandom.find(randomIndex) != alreadySelectedRandom.end()) {
            randomIndex = generateNumber(0, (int) pointsVector.itemValues.size() - 1);
        }
        alreadySelectedRandom[randomIndex] = true;
        centers[i].first = pointsVector.itemValues[randomIndex].first; //Assign randomly the ItemID of a point of pointVector
        centers[i].second = pointsVector.itemValues[randomIndex].second; //Assign randomly the coordinates of a point of pointVector
    }
}

template<class inputData>
void VectorKMEANS<inputData>::AssignmentSimplest(InputGenericVector<int> const &pointsVector) {

    //For each assignment erase the vector of clusters
    clusters.clear();
    clusters.shrink_to_fit();
    clusters.resize(k);
    //Assignment:For each point we find the nearest center using l1 metric(manhattan distance)
    for (unsigned long point = 0; point < pointsVector.itemValues.size(); ++point) {
        double min = manhattanDistance(pointsVector.itemValues[point].second, centers[0].second);
        unsigned int minCentroidIndex = 0;
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

}

template<class inputData>
void VectorKMEANS<inputData>::ReverseAssignmentPreload(InputGenericVector<int> &pointsVector, int const &k_vec_given,
                                                       int const &L_given) {
    //ExactNeighboursVector<int> inputNeighboursVector(pointsVector,pointsVector, true);

    double w = 8436.48;//inputNeighboursVector.wCalculator();
    //cout<<w<<endl;
    //exit(1);

    double radius = numeric_limits<double>::max();
    for (unsigned int i = 0; i < k; ++i) {
        for (unsigned int j = 0; j < k; ++j) {
            if (i != j) {
                double temp = manhattanDistance(centers[i].second, centers[j].second);
                if (temp < radius)
                    radius = temp;
            }
        }
    }
    radius = radius / 2;

    //Create the LSH structure
    lsh = new LSH(k_vec_given, L_given, w, pointsVector.itemValues[0].second.size(), radius);

    //Assign points in LSH tables
    for (unsigned long i = 0; i < pointsVector.itemValues.size(); ++i) {
        for (unsigned int numberHashtable = 0; numberHashtable < lsh->L; numberHashtable++) {
            unsigned int g = lsh->hashFunctions[numberHashtable].gCalculator(pointsVector.itemValues[i].second,
                                                                             (unsigned int) lsh->k_vec, w);
            lsh->hashTables->insertHashtable(numberHashtable, g, pointsVector.itemValues[i], i);
        }
        assignedItems[pointsVector.itemValues[i].first] = make_tuple(-1, -1, i);
    }
    cout << "komple " << assignedItems.size() << endl;
}

template<class inputData>
void VectorKMEANS<inputData>::ReverseAssignment(InputGenericVector<int> const &pointsVector) {
    //For each assignment erase the vector of clusters
    clusters.clear();
    clusters.shrink_to_fit();
    clusters.resize(k);

    unsigned int clustered_items = 0;
    auto conditionSuccessAmount = (unsigned int) round(0.7 * pointsVector.itemValues.size());
    cout << "success " << conditionSuccessAmount << endl;
    while (clustered_items < conditionSuccessAmount) {
        cout << "3ana!! " << lsh->radius << " " << clustered_items << endl;
        //In 1 out of 100 times there is a possibility points to be scattered in different buckets than the center so although we increase the radius,there are not any more points in the buckets.
        //So we break the loop and bruteforce the assignment :/
        if (lsh->radius == numeric_limits<double>::infinity()) {
            break;
        }
        for (unsigned int i = 0; i < k; ++i) {

            lsh->hashTables->rangeSearch(centers[i], lsh->radius, lsh->hashFunctions, (unsigned int) lsh->k_vec,
                                         lsh->w, clusters, i, assignedItems);

            clusters[i].centroid = centers[i].first;
            clusters[i].centroidCoordinates = centers[i].second;
            clustered_items = 0;
            for (unsigned int cluster = 0; cluster < k; ++cluster) {
                clustered_items += clusters[cluster].ItemIDs.size();
            }

        }
        lsh->radius *= 2;
    }

    //Assign the points which were not assigned by range search
    for (auto itr = assignedItems.begin(); itr != assignedItems.end(); itr++) {
        if (get<0>(itr->second) == -1) {
            double min = manhattanDistance(pointsVector.itemValues[get<2>(itr->second)].second, centers[0].second);
            unsigned int minCentroidIndex = 0;
            for (unsigned int centroid = 1; centroid < k; ++centroid) {
                double distance_from_centroid = manhattanDistance(pointsVector.itemValues[get<2>(itr->second)].second,
                                                                  centers[centroid].second);
                if (min > distance_from_centroid) {
                    min = distance_from_centroid;
                    minCentroidIndex = centroid;
                }
            }

            clusters[minCentroidIndex].ItemIDs.push_back(pointsVector.itemValues[get<2>(itr->second)].first);
            clusters[minCentroidIndex].indexes.push_back(get<2>(itr->second));
        } else {
            //Unassign points for new centers
            get<0>(itr->second) = -1;
            get<1>(itr->second) = -1;
        }
    }
    //lsh->hashTables->unassignedPointsHandling(clusters,centers,k);
    unsigned int howmany = 0;
    for (auto itr = assignedItems.begin(); itr != assignedItems.end(); itr++) {
        if (get<0>(itr->second) != -1)
            howmany++;
    }

    //Note to me:Delete later
//cout<<"how many "<<howmany<<endl;
//    cout<<"clustered "<<clustered_items<<endl;
//    clustered_items=0;
//    for(unsigned int i=0; i<k; i++){
//        clustered_items+=clusters[i].ItemIDs.size();
//    }
//    cout<<"clustered "<<clustered_items<<endl;
//
//    for (unsigned int i = 0; i < k; ++i) {
//        cout << "CLUSTER-" << i + 1 << clusters[i].centroid << " {";
//        for (unsigned long item = 0; item < clusters[i].ItemIDs.size(); ++item) {
//            if (item == clusters[i].ItemIDs.size() - 1) {
//                cout << clusters[i].ItemIDs[item] << "}" << endl;
//            } else
//                cout << clusters[i].ItemIDs[item] << ", ";
//        }
//    }
    //exit(1);
}

template<class inputData>
void VectorKMEANS<inputData>::UpdateSimplest(InputGenericVector<int> const &pointsVector, bool &unchangedCenters) {
    unchangedCenters = true;
    for (unsigned int i = 0; i < k; ++i) {
        vector<int> temp;
        temp.resize(pointsVector.itemValues[0].second.size());
        if (!clusters[i].indexes.empty()) {
            //Calculate the mean of each dimension
            for (unsigned long j = 0; j < temp.size(); ++j) {
                for (auto index:clusters[i].indexes) {
                    temp[j] += pointsVector.itemValues[index].second[j];
                }
                temp[j] = temp[j] /
                          (int) clusters[i].indexes.size(); //Pithano segmentation fault an to i-osto cluster den exei kanena stoixeio
            }
        } else
            temp = pointsVector.itemValues[generateNumber(0, (int) pointsVector.itemValues.size() -
                                                             1)].second; //Assign randomly the coordinates of a point of pointVector
        if (temp != centers[i].second) {
            unchangedCenters = false;
            centers[i].first = "OutofDataset";
            centers[i].second = temp;
        }
    }
}

template<class inputData>
VectorKMEANS<inputData>::VectorKMEANS(InputGenericVector<int> &pointsVector, unsigned int const &k_given,
                                      unsigned int const &whichInitialization, unsigned int const &whichAssignment,
                                      unsigned int const &whichUpdate) {
    k = k_given;
    if (whichInitialization == 1)
        InitializationSimplest(pointsVector);
    else
        cout << "foo" << endl;

    //In this preload we create the lsh structures and put all points in the hash tables in order to be queried by the centroids
    if (whichAssignment == 2)
        ReverseAssignmentPreload(pointsVector, 4, 3);

    bool unchangedCenters = false;
    //If the centers do not change we are done.
    while (!unchangedCenters) {

        if (whichAssignment == 1)
            AssignmentSimplest(pointsVector);
        else
            ReverseAssignment(pointsVector);

        if (whichUpdate == 2)
            UpdateSimplest(pointsVector, unchangedCenters);
        else
            cout << "foo" << endl;

    }

    for (unsigned int i = 0; i < k; ++i) {
        cout << "CLUSTER-" << i + 1 << clusters[i].centroid << " {";
//            for(unsigned int item=0; item<clusters[i].centroidCoordinates.size(); ++item){
//                cout<<clusters[i].centroidCoordinates[item]<<" ";
//            }
//            cout<<endl;
//            cout<<"Items"<<endl;
        for (unsigned long item = 0; item < clusters[i].ItemIDs.size(); ++item) {
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
    if (whichAssignment == 2)
        free(lsh);
}

template
class VectorKMEANS<int>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.