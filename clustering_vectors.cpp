#include "clustering_vectors.h"

int generateNumberV(int const &range_from,
                   int const &range_to) {      //https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution

    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_int_distribution<> distr(range_from, range_to);

    return distr(generator);

}

template<class inputData>
inline double
VectorClustering<inputData>::manhattanDistance(vector<inputData> const &point, vector<inputData> const &query) {
    double distance = 0;
    for (unsigned long i = 0; i < point.size(); ++i) {
        distance += abs(point[i] - query[i]);
    }
    return distance;
}

template<class inputData>
inline double VectorClustering<inputData>::fast_distance_calculationV(int const &index1, int const &index2,
                                                                      vector<inputData> const &item1,
                                                                      vector<inputData> const &item2) {

    //Check if we had already calculated this distance somewhere so we have it saved and don't need to recalculate it
    auto iterator_to_calculatedDistances = calculatedDistances.find(make_pair(index1, index2));
    if (iterator_to_calculatedDistances == calculatedDistances.end()) {
        double temp_dist = manhattanDistance(item1, item2);
        if (index1 != -1 && index2 != -1)
            calculatedDistances[make_pair(index1, index2)] = temp_dist;
        return temp_dist;
    } else {
        return iterator_to_calculatedDistances->second;
    }
}
template<class inputData>
void VectorClustering<inputData>::InitializationSimplest(InputGenericVector<inputData> const &pointsVector) {
    centers.resize(k);
    //We will use an unordered map so if a random value has been already selected to select another one
    unordered_map<int, bool> alreadySelectedRandom;
    for (unsigned int i = 0; i < k; ++i) {
        int randomIndex = generateNumberV(0, (int) pointsVector.itemValues.size() - 1);
        //While generated index has already been used, generate another one
        while (alreadySelectedRandom.find(randomIndex) != alreadySelectedRandom.end()) {
            randomIndex = generateNumberV(0, (int) pointsVector.itemValues.size() - 1);
        }
        alreadySelectedRandom[randomIndex] = true;
        get<0>(centers[i]) = pointsVector.itemValues[randomIndex].first; //Assign randomly the ItemID of a point of pointVector
        get<1>(centers[i]) = pointsVector.itemValues[randomIndex].second; //Assign randomly the coordinates of a point of pointVector
        get<2>(centers[i]) = randomIndex;
    }
}

template<class inputData>
void VectorClustering<inputData>::InitializationKmeansPlusPlus(InputGenericVector<inputData> const &pointsVector) {
    vector<int> indexes;                            //vector of the places of pointsVector as centers
    centers.resize(k);
    chooseRandomCenter(pointsVector, indexes);
    get<0>(centers[0]) = pointsVector.itemValues[indexes[0]].first; //Assign randomly the ItemID of a point of pointVector
    get<1>(centers[0]) = pointsVector.itemValues[indexes[0]].second; //Assign randomly the coordinates of a point of pointVector
    get<2>(centers[0]) = indexes[0];

    for (unsigned int i = 1; i < k; i++) {
        createNewCenter(pointsVector, indexes);
        get<0>(centers[i]) = pointsVector.itemValues[indexes[i]].first; //Assign randomly the ItemID of a point of pointVector
        get<1>(centers[i]) = pointsVector.itemValues[indexes[i]].second; //Assign randomly the coordinates of a point of pointVector
        get<2>(centers[i]) = indexes[i];
    }
}
template<class inputData>
void VectorClustering<inputData>::AssignmentSimplest(InputGenericVector<inputData> const &pointsVector) {

    //For each assignment erase the vector of clusters
    clusters.clear();
    clusters.shrink_to_fit();
    clusters.resize(k);
    //Assignment:For each point we find the nearest center using l1 metric(manhattan distance)
    for (unsigned long point = 0; point < pointsVector.itemValues.size(); ++point) {
        double min = fast_distance_calculationV(point, get<2>(centers[0]), pointsVector.itemValues[point].second,
                                                get<1>(centers[0]));
        unsigned int minCentroidIndex = 0;
        for (unsigned int centroid = 1; centroid < k; ++centroid) {
            double distance_from_centroid = fast_distance_calculationV(point, get<2>(centers[centroid]),
                                                                       pointsVector.itemValues[point].second,
                                                                       get<1>(centers[centroid]));
            if (min > distance_from_centroid) {
                min = distance_from_centroid;
                minCentroidIndex = centroid;
            }
        }

        clusters[minCentroidIndex].centroid = get<0>(centers[minCentroidIndex]);
        clusters[minCentroidIndex].centroidCoordinates = get<1>(centers[minCentroidIndex]);
        clusters[minCentroidIndex].ItemIDs.push_back(pointsVector.itemValues[point].first);
        clusters[minCentroidIndex].indexes.push_back(point);
    }

}

template<class inputData>
void
VectorClustering<inputData>::ReverseAssignmentPreload(InputGenericVector<inputData> &pointsVector,
                                                      int const &k_vec_given,
                                                      int const &L_given) {
    //ExactNeighboursVector<int> inputNeighboursVector(pointsVector,pointsVector, true);

    double w = 8436.48;//inputNeighboursVector.wCalculator();
    //cout<<w<<endl;
    //exit(1);

    double radius = numeric_limits<double>::max();
    for (unsigned int i = 0; i < k; ++i) {
        for (unsigned int j = 0; j < k; ++j) {
            if (i != j) {
                double temp = fast_distance_calculationV(get<2>(centers[i]), get<2>(centers[j]), get<1>(centers[i]),
                                                         get<1>(centers[j]));
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
            lsh->hashTables->insertHashtable(numberHashtable, g, i);
        }
        assignedItems[i] = make_pair(-1, -1);
    }
    cout << "komple " << assignedItems.size() << endl;
}

template<class inputData>
void VectorClustering<inputData>::ReverseAssignment(InputGenericVector<inputData> const &pointsVector) {
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
                                         lsh->w, clusters, i, assignedItems, pointsVector, calculatedDistances);

            clusters[i].centroid = get<0>(centers[i]);
            clusters[i].centroidCoordinates = get<1>(centers[i]);
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
            double min = fast_distance_calculationV(itr->first, get<2>(centers[0]),
                                                    pointsVector.itemValues[itr->first].second, get<1>(centers[0]));
            unsigned int minCentroidIndex = 0;
            for (unsigned int centroid = 1; centroid < k; ++centroid) {
                double distance_from_centroid = fast_distance_calculationV(itr->first, get<2>(centers[centroid]),
                                                                           pointsVector.itemValues[itr->first].second,
                                                                           get<1>(centers[centroid]));
                if (min > distance_from_centroid) {
                    min = distance_from_centroid;
                    minCentroidIndex = centroid;
                }
            }
            clusters[minCentroidIndex].centroid = get<0>(centers[minCentroidIndex]);
            clusters[minCentroidIndex].centroidCoordinates = get<1>(centers[minCentroidIndex]);
            clusters[minCentroidIndex].ItemIDs.push_back(pointsVector.itemValues[itr->first].first);
            clusters[minCentroidIndex].indexes.push_back(itr->first);
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
void
VectorClustering<inputData>::UpdateSimplest(InputGenericVector<inputData> const &pointsVector, bool &unchangedCenters) {
    unchangedCenters = true;
    for (unsigned int i = 0; i < k; ++i) {
        vector<inputData> temp;
        temp.resize(pointsVector.itemValues[0].second.size());
        if (!clusters[i].indexes.empty()) {
            //Calculate the mean of each dimension
            for (unsigned long j = 0; j < temp.size(); ++j) {
                for (auto index:clusters[i].indexes) {
                    temp[j] += pointsVector.itemValues[index].second[j];
                }
                temp[j] = temp[j] / (int) clusters[i].indexes.size();
            }
        } else {
            //Assign randomly the coordinates of a point of pointVector
            int randomIndex = generateNumberV(0, (int) pointsVector.itemValues.size() - 1);

            //Check if random value already exists in the centers
            while (true) {
                for (unsigned int cluster = 0; cluster < k; ++cluster) {
                    if (get<0>(centers[cluster]) == pointsVector.itemValues[randomIndex].first) {
                        randomIndex = generateNumberV(0, (int) pointsVector.itemValues.size() - 1);
                        continue;
                    }
                }
                break;
            }
            get<0>(centers[i]) = pointsVector.itemValues[randomIndex].first; //Assign randomly the ItemID of a point of pointVector
            get<1>(centers[i]) = pointsVector.itemValues[randomIndex].second; //Assign randomly the coordinates of a point of pointVector
            get<2>(centers[i]) = randomIndex;
            unchangedCenters = false;
            continue;
        }
        if (temp != get<1>(centers[i])) {
            unchangedCenters = false;
            get<0>(centers[i]) = "OutofDataset";
            get<1>(centers[i]) = temp;
            get<2>(centers[i]) = -1;
        }
    }
}

template<class inputData>
void
VectorClustering<inputData>::UpdateALaLoyd(InputGenericVector<inputData> const &pointsVector, bool &unchangedCenters) {
    unchangedCenters = true;
    for (unsigned int i = 0; i < k; ++i) {
        double min_sum = numeric_limits<double>::max();
        int min_index = -1;
        for (unsigned long no_index1 = 0; no_index1 < clusters[i].indexes.size(); ++no_index1) {
            double temp_sum = 0;
            for (auto index2:clusters[i].indexes) {
                temp_sum += fast_distance_calculationV(clusters[i].indexes[no_index1], index2,
                                                       pointsVector.itemValues[clusters[i].indexes[no_index1]].second,
                                                       pointsVector.itemValues[index2].second);
            }
            if (min_sum > temp_sum) {
                min_sum = temp_sum;
                min_index = clusters[i].indexes[no_index1];
            }
        }
        cout << "min index " << min_index << endl;
        //If empty cluster assign at random
        if (min_index == -1) {
            int randomIndex = generateNumberV(0, (int) pointsVector.itemValues.size() - 1);

            //Check if random value already exists in the centers
            while (true) {
                for (unsigned int cluster = 0; cluster < k; ++cluster) {
                    if (get<0>(centers[cluster]) == pointsVector.itemValues[randomIndex].first) {
                        randomIndex = generateNumberV(0, (int) pointsVector.itemValues.size() - 1);
                        continue;
                    }
                }
                break;
            }
            get<0>(centers[i]) = pointsVector.itemValues[randomIndex].first; //Assign randomly the ItemID of a point of pointVector
            get<1>(centers[i]) = pointsVector.itemValues[randomIndex].second; //Assign randomly the coordinates of a point of pointVector
            get<2>(centers[i]) = randomIndex;
            unchangedCenters = false;
            continue;
        }
        if (pointsVector.itemValues[min_index].second != get<1>(centers[i])) {
            unchangedCenters = false;
            get<0>(centers[i]) = pointsVector.itemValues[min_index].first;
            get<1>(centers[i]) = pointsVector.itemValues[min_index].second;
            get<2>(centers[i]) = min_index;
        }

    }
}

template<class inputData>
void VectorClustering<inputData>::Silhouette(InputGenericVector<inputData> &pointsVector) {
    double silhouete_total = 0;
    cout << "Silhouette: [";
    for (unsigned int i = 0; i < k; ++i) {
        double silhouete_of_cluster = 0;
        for (unsigned long item = 0; item < clusters[i].ItemIDs.size(); ++item) {
            double silhouete_of_item = 0;
            double a = 0;
            for (unsigned long item_in_cluster = 0; item_in_cluster < clusters[i].ItemIDs.size(); ++item_in_cluster) {
                if (item == item_in_cluster)
                    continue;
                a += fast_distance_calculationV(clusters[i].indexes[item], clusters[i].indexes[item_in_cluster],
                                                pointsVector.itemValues[clusters[i].indexes[item]].second,
                                                pointsVector.itemValues[clusters[i].indexes[item_in_cluster]].second);
            }
            if (clusters[i].ItemIDs.

                    size()

                > 1)
                a /= clusters[i].ItemIDs.

                        size()

                     - 1;
            unsigned int second_best_cluster = i;
            double min = numeric_limits<double>::max();
            for (
                    unsigned int cluster = 0;
                    cluster < k;
                    cluster++) {
                if (cluster == i || clusters[cluster].ItemIDs.

                        empty()

                        )
                    continue;
                double temp = manhattanDistance(pointsVector.itemValues[clusters[i].indexes[item]].second,
                                                clusters[cluster].centroidCoordinates);
                if (min > temp) {
                    min = temp;
                    second_best_cluster = cluster;
                }
            }

            double b = 0;
            for (unsigned long item_in_cluster = 0;
                 item_in_cluster < clusters[second_best_cluster].ItemIDs.size(); ++item_in_cluster) {
                b += fast_distance_calculationV(clusters[i].indexes[item], second_best_cluster,
                                                   pointsVector.itemValues[clusters[i].indexes[item]].second,
                                                   pointsVector.itemValues[clusters[second_best_cluster].indexes[item_in_cluster]].second);
            }
            if (!clusters[second_best_cluster].ItemIDs.empty())
                b /= clusters[second_best_cluster].ItemIDs.size();

            silhouete_of_item = b - a;
            if (b > a)
                silhouete_of_item /=
                        b;
            else
                silhouete_of_item /=
                        a;
            silhouete_of_cluster +=
                    silhouete_of_item;
        }
        if (!clusters[i].ItemIDs.

                empty()

                )
            silhouete_of_cluster /= clusters[i].ItemIDs.

                    size();

        silhouete_total +=
                silhouete_of_cluster;
        cout << silhouete_of_cluster << ",";
    }
    silhouete_total /=
            k;
    cout << " " << silhouete_total << "]" <<
         endl;
}

template<class inputData>
void VectorClustering<inputData>::Printing(unsigned int const &whichInitialization, unsigned int const &whichAssignment,
                                           unsigned int const &whichUpdate, bool const &complete,
                                           double const &duration, InputGenericVector<inputData> &pointsVector) {
    cout << "Algorithm: I" << whichInitialization << "A" << whichAssignment << "U" << whichUpdate << endl;
    for (unsigned int i = 0; i < k; ++i) {
        cout << "CLUSTER-" << i + 1 << " {size: " << clusters[i].ItemIDs.size() << ", centroid: ";
        if ("OutofDataset" != clusters[i].centroid)
            cout << clusters[i].centroid;
        else {
            for (auto const &coord:clusters[i].centroidCoordinates)
                cout << coord << " ";
        }
        cout << "}" << endl;
    }

    cout << "Clustering_time: " << duration << endl;

    Silhouette(pointsVector);

    cout << endl;

    if (complete) {
        for (unsigned int i = 0; i < k; ++i) {
            cout << "CLUSTER-" << i + 1 << " {";
            for (unsigned long item = 0; item < clusters[i].ItemIDs.size(); ++item) {
                if (item == clusters[i].ItemIDs.size() - 1) {
                    cout << clusters[i].ItemIDs[item] << "}" << endl;
                } else
                    cout << clusters[i].ItemIDs[item] << ", ";
            }
        }
    }
}

template<class inputData>
VectorClustering<inputData>::VectorClustering(InputGenericVector<inputData> &pointsVector, unsigned int const &k_given,
                                              unsigned int const &whichInitialization,
                                              unsigned int const &whichAssignment,
                                              unsigned int const &whichUpdate, unsigned int const &k_of_lsh,
                                              unsigned int const &L_hashtables, bool const &complete) {
    k = k_given;
    auto start = std::chrono::system_clock::now();
    if (whichInitialization == 1)
        InitializationSimplest(pointsVector);
    else
        InitializationKmeansPlusPlus(pointsVector);

    //In this preload we create the lsh structures and put all points in the hash tables in order to be queried by the centroids
    if (whichAssignment == 2)
        ReverseAssignmentPreload(pointsVector, k_of_lsh, L_hashtables);

    bool unchangedCenters = false;
    //If the centers do not change we are done. We have the min objective function.
    while (!unchangedCenters) {

        if (whichAssignment == 1)
            AssignmentSimplest(pointsVector);
        else
            ReverseAssignment(pointsVector);

        if (whichUpdate == 2)
            UpdateSimplest(pointsVector, unchangedCenters);
        else
            UpdateALaLoyd(pointsVector, unchangedCenters);

    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> clustering_duration = end - start;

    if (whichAssignment == 2)
        free(lsh);

    Printing(whichInitialization, whichAssignment, whichUpdate, complete, clustering_duration.count(), pointsVector);



}

template
class VectorClustering<int>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.
template
class VectorClustering<double>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.