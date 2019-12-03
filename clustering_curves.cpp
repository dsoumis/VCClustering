#include "clustering_curves.h"

int generateNumberC(int const &range_from,
                    int const &range_to) {      //https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution

    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_int_distribution<> distr(range_from, range_to);

    return distr(generator);

}

template<class inputData>
inline double CurveClustering<inputData>::fast_distance_calculationC(int const &index1, int const &index2,
                                                                     vector<inputData> const &item1,
                                                                     vector<inputData> const &item2) {
    vector<vector<Cell>> tempFoo;
    //Check if we had already calculated this distance somewhere so we have it saved and don't need to recalculate it
    auto iterator_to_calculatedDistances = calculatedDistances.find(make_pair(index1, index2));
    if (iterator_to_calculatedDistances == calculatedDistances.end()) {
        double temp_dist = Dtw(item1, item2, tempFoo);
        if (index1 != -1 && index2 != -1)
            calculatedDistances[make_pair(index1, index2)] = temp_dist;
        return temp_dist;
    } else {
        return iterator_to_calculatedDistances->second;
    }
}

template<class inputData>
void CurveClustering<inputData>::InitializationSimplest(InputGenericVector<inputData> const &curvesVector) {
    centers.resize(k);
    //We will use an unordered map so if a random value has been already selected to select another one
    unordered_map<int, bool> alreadySelectedRandom;
    for (unsigned int i = 0; i < k; ++i) {
        int randomIndex = generateNumberC(0, (int) curvesVector.itemValues.size() - 1);
        //While generated index has already been used, generate another one
        while (alreadySelectedRandom.find(randomIndex) != alreadySelectedRandom.end()) {
            randomIndex = generateNumberC(0, (int) curvesVector.itemValues.size() - 1);
        }
        alreadySelectedRandom[randomIndex] = true;
        get<0>(centers[i]) = curvesVector.itemValues[randomIndex].first; //Assign randomly the ItemID of a point of pointVector
        get<1>(centers[i]) = curvesVector.itemValues[randomIndex].second; //Assign randomly the coordinates of a point of pointVector
        get<2>(centers[i]) = randomIndex;
    }
}

template<class inputData>
void CurveClustering<inputData>::InitializationKmeansPlusPlus(InputGenericVector<inputData> const &curvesVector) {
    vector<int> indexes;                            //vector of the places of pointsVector as centers
    centers.resize(k);
    chooseRandomCenter(curvesVector, indexes);
    get<0>(centers[0]) = curvesVector.itemValues[indexes[0]].first; //Assign randomly the ItemID of a point of pointVector
    get<1>(centers[0]) = curvesVector.itemValues[indexes[0]].second; //Assign randomly the coordinates of a point of pointVector
    get<2>(centers[0]) = indexes[0];

    for (unsigned int i = 1; i < k; i++) {
        createNewCenter(curvesVector, indexes);
        get<0>(centers[i]) = curvesVector.itemValues[indexes[i]].first; //Assign randomly the ItemID of a point of pointVector
        get<1>(centers[i]) = curvesVector.itemValues[indexes[i]].second; //Assign randomly the coordinates of a point of pointVector
        get<2>(centers[i]) = indexes[i];
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
        double min = fast_distance_calculationC(curve, get<2>(centers[0]), curvesVector.itemValues[curve].second,
                                                get<1>(centers[0]));

        unsigned int minCentroidIndex = 0;
        for (unsigned int centroid = 1; centroid < k; ++centroid) {
            double distance_from_centroid = fast_distance_calculationC(curve, get<2>(centers[centroid]),
                                                                       curvesVector.itemValues[curve].second,
                                                                       get<1>(centers[centroid]));

            if (min > distance_from_centroid) {
                min = distance_from_centroid;
                minCentroidIndex = centroid;
            }
        }

        clusters[minCentroidIndex].centroid = get<0>(centers[minCentroidIndex]);
        clusters[minCentroidIndex].centroidCoordinates = get<1>(centers[minCentroidIndex]);
        clusters[minCentroidIndex].ItemIDs.push_back(curvesVector.itemValues[curve].first);
        clusters[minCentroidIndex].indexes.push_back(curve);
    }
}

double deltaCalculator(InputGenericVector<pair<double, double>> const &curvesVector) {
    double delta = 0;
    for (auto const &curve:curvesVector.itemValues) {
        double temp = 0;
        for (unsigned long coord = 0; coord < curve.second.size() - 1; coord++) {
            temp += sqrt((curve.second[coord].first - curve.second[coord + 1].first) *
                         (curve.second[coord].first - curve.second[coord + 1].first)
                         + (curve.second[coord].second - curve.second[coord + 1].second) *
                           (curve.second[coord].second - curve.second[coord + 1].second));   //Euclidean distance
        }
        temp /= curve.second.size();
        delta += temp;
    }
    delta /= curvesVector.itemValues.size();
    return delta;
}

template<class inputData>
void CurveClustering<inputData>::ReverseAssignmentPreload(InputGenericVector<inputData> &curvesVector,
                                                          int const &k_vec_given, int const &L_given,
                                                          unsigned int const &dimensions) {
    double delta = deltaCalculator(curvesVector);
    double max_coord = 0; //Consider max coordinate to be zero
    curvesVector.maxCoordFinder(max_coord); //Find  the max coordinate to use it in vector padding
    max_coord *= 100;
    max_coord /= delta;
    vector<vector<Cell>> foo;
    double radius = numeric_limits<double>::max();
    for (unsigned int i = 0; i < k; ++i) {
        for (unsigned int j = 0; j < k; ++j) {
            if (i != j) {
                double temp = fast_distance_calculationC(get<2>(centers[i]), get<2>(centers[j]), get<1>(centers[i]),
                                                         get<1>(centers[j]));
                if (temp < radius)
                    radius = temp;
            }
        }
    }
    radius = radius / 2;

    //Create the LSH structure
    lsh = new LSH(k_vec_given, L_given, radius, minCurveSize, maxCurveSize, dimensions, delta, curvesVector, max_coord);

    //Assign curves in Grids
    for (unsigned long curve = 0; curve < curvesVector.itemValues.size(); curve++) {
        for (unsigned int i = 0; i < lsh->L_grid; i++) {
            //Hash the concatenated vector to the 1 LSH hash table of each grid
            unsigned int g = lsh->hashFunctions[i].gCalculator(lsh->concatenated_gridCurve_vectors[i][curve],
                                                               lsh->k_vec, lsh->w[i]);
            lsh->hashTables[i].insertHashtable(0, g, curve);
        }
        assignedItems[curve] = make_pair(-1, -1);
    }
    cout << "Curves assigned to grids  " << assignedItems.size() << endl;
}

template<class inputData>
void CurveClustering<inputData>::ReverseAssignment(InputGenericVector<inputData> const &curvesVector) {
    //For each assignment erase the vector of clusters
    clusters.clear();
    clusters.shrink_to_fit();
    clusters.resize(k);

    unsigned int clustered_items = 0;
    auto conditionSuccessAmount = (unsigned int) round(0.7 * curvesVector.itemValues.size());
    while (clustered_items < conditionSuccessAmount) {
        //In 1 out of 100 times there is a possibility points to be scattered in different buckets than the center so although we increase the radius,there are not any more points in the buckets.
        //So we break the loop and bruteforce the assignment :/
        if (lsh->radius == numeric_limits<double>::infinity()) {
            break;
        }
        for (unsigned int i = 0; i < k; ++i) {

            vector<double> concatenated_gridCurve_vector;
            for (unsigned int grid = 1; grid < lsh->L_grid; grid++) {
                //Create the concatenated grid curve vector for each curve of each grid
                concatenated_gridCurve_vector = lsh->grids[grid].concatenated_vector_from_gridCurve(get<1>(centers[i]),
                                                                                                    maxCurveSize,
                                                                                                    lsh->max_coord);
                //Hash the concatenated vector to the 1 LSH hash table of each grid
                unsigned int g = lsh->hashFunctions[grid].gCalculator(concatenated_gridCurve_vector, lsh->k_vec,
                                                                      lsh->w[grid]);
                lsh->hashTables[grid].rangeSearch(centers[i], lsh->radius, g, clusters, i, assignedItems, curvesVector,
                                                  calculatedDistances);

            }

            clusters[i].centroid = get<0>(centers[i]);
            clusters[i].centroidCoordinates = get<1>(centers[i]);
            clustered_items = 0;
            for (unsigned int cluster = 0; cluster < k; ++cluster) {
                clustered_items += clusters[cluster].ItemIDs.size();
            }

        }
        lsh->radius *= 2;
    }
    vector<vector<Cell>> foo;
    //Assign the points which were not assigned by range search
    for (auto itr = assignedItems.begin(); itr != assignedItems.end(); itr++) {
        if (get<0>(itr->second) == -1) {
            double min = fast_distance_calculationC(itr->first, get<2>(centers[0]),
                                                    curvesVector.itemValues[itr->first].second, get<1>(centers[0]));
            unsigned int minCentroidIndex = 0;
            for (unsigned int centroid = 1; centroid < k; ++centroid) {
                double distance_from_centroid = fast_distance_calculationC(itr->first, get<2>(centers[centroid]),
                                                                           curvesVector.itemValues[itr->first].second,
                                                                           get<1>(centers[centroid]));
                if (min > distance_from_centroid) {
                    min = distance_from_centroid;
                    minCentroidIndex = centroid;
                }
            }

            clusters[minCentroidIndex].centroid = get<0>(centers[minCentroidIndex]);
            clusters[minCentroidIndex].centroidCoordinates = get<1>(centers[minCentroidIndex]);
            clusters[minCentroidIndex].ItemIDs.push_back(curvesVector.itemValues[itr->first].first);
            clusters[minCentroidIndex].indexes.push_back(itr->first);
        } else {
            //Unassign curves for new centers
            get<0>(itr->second) = -1;
            get<1>(itr->second) = -1;
        }
    }

    unsigned int howmany = 0;
    for (auto itr = assignedItems.begin(); itr != assignedItems.end(); itr++) {
        if (get<0>(itr->second) != -1)
            howmany++;
    }

}

template<class inputData>
void CurveClustering<inputData>::InitializationUpdate(unsigned int const &cluster_index,
                                                      InputGenericVector<inputData> const &curvesVector,
                                                      unsigned long &lamda, vector<inputData> &C) {
    lamda = 0;
    //Add the length of each curve which exists in cluster by indexing them in the original GenericVector
    for (unsigned long curve = 0; curve < clusters[cluster_index].ItemIDs.size(); ++curve) {
        lamda += curvesVector.itemValues[clusters[cluster_index].indexes[curve]].second.size();
    }
    lamda /= clusters[cluster_index].ItemIDs.size();
    //Random index in cluster
    int randomIndex = generateNumberC(0, (int) clusters[cluster_index].ItemIDs.size() - 1);
    //Check if the sequence's length is >lamda by indexing in the original GenericVector using the saved index of curve which exists in cluster
    while (curvesVector.itemValues[clusters[cluster_index].indexes[randomIndex]].second.size() < lamda) {
        randomIndex = generateNumberC(0, (int) clusters[cluster_index].ItemIDs.size() - 1);
    }

    //If curve's length is already lamda assign the whole sequence of coordinates
    if (curvesVector.itemValues[clusters[cluster_index].indexes[randomIndex]].second.size() == lamda) {
        C = curvesVector.itemValues[clusters[cluster_index].indexes[randomIndex]].second;
    }
        //If curve's length is greater than lamda generate an index in sequence of coordinates
    else {
        int randomSubsequenceIndex = generateNumberC(0,
                                                     (int) curvesVector.itemValues[clusters[cluster_index].indexes[randomIndex]].second.size() -
                                                     1);
        //check if points after index are enough to successfully create the subsequence. index + point after it must equal lamda
        while (true) {

            unsigned int subsequence_lentgh = 0;
            for (unsigned long ind = randomSubsequenceIndex;
                 ind < curvesVector.itemValues[clusters[cluster_index].indexes[randomIndex]].second.size(); ++ind) {
                subsequence_lentgh++;
            }
            if (subsequence_lentgh == lamda) {
                //Assign the lamda-subsequence
                for (unsigned long ind = randomSubsequenceIndex;
                     ind < curvesVector.itemValues[clusters[cluster_index].indexes[randomIndex]].second.size(); ++ind) {
                    C.push_back(curvesVector.itemValues[clusters[cluster_index].indexes[randomIndex]].second[ind]);
                }
                break;
            } else {
                randomSubsequenceIndex = generateNumberC(0,
                                                         (int) curvesVector.itemValues[clusters[cluster_index].indexes[randomIndex]].second.size() -
                                                         1);
            }
        }
    }
}

template<class inputData>
void
CurveClustering<inputData>::UpdateSimplest(InputGenericVector<inputData> const &curvesVector, bool &unchangedCenters) {
    unchangedCenters = true;
    vector<vector<Cell>> dummy;
    for (unsigned int i = 0; i < k; ++i) {
        unsigned long lamda;
        vector<inputData> C;
        if (!clusters[i].indexes.empty()) {
            vector<inputData> C_;

            InitializationUpdate(i, curvesVector, lamda, C);

            do {
                C_ = C;
                vector<vector<pair<double, double>>> A;
                A.resize(lamda);
                vector<vector<Cell>> temp;
                for (unsigned long curve = 0; curve < clusters[i].ItemIDs.size(); ++curve) {
                    Dtw(C, curvesVector.itemValues[clusters[i].indexes[curve]].second, temp);
                    vector<pair<int, int>> IPairs = printPath(temp);
                    for (auto const &pair:IPairs) {
                        A[pair.first].push_back(
                                curvesVector.itemValues[clusters[i].indexes[curve]].second[pair.second]);
                    }
                }

                for (unsigned long j = 0; j < lamda; ++j) {
                    pair<double, double> meanA;
                    meanA.first = 0;
                    meanA.second = 0;
                    for (auto const &pair:A[j]) {
                        meanA.first += pair.first;
                        meanA.second += pair.second;
                    }
                    meanA.first /= A[j].size();
                    meanA.second /= A[j].size();
                    C[j] = meanA;
                }

            } while (C_ != C && Dtw(C, C_, dummy) > 0.5);
        } else {
            //Assign randomly the coordinates of a curve of curvesVector
            int randomIndex = generateNumberC(0, (int) curvesVector.itemValues.size() - 1);

            //Check if random value already exists in the centers
            while (true) {
                for (unsigned int cluster = 0; cluster < k; ++cluster) {
                    if (get<0>(centers[cluster]) == curvesVector.itemValues[randomIndex].first) {
                        randomIndex = generateNumberC(0, (int) curvesVector.itemValues.size() - 1);
                        continue;
                    }
                }
                break;
            }
            get<0>(centers[i]) = curvesVector.itemValues[randomIndex].first; //Assign randomly the ItemID of a point of pointVector
            get<1>(centers[i]) = curvesVector.itemValues[randomIndex].second; //Assign randomly the coordinates of a point of pointVector
            get<2>(centers[i]) = randomIndex;
            unchangedCenters = false;
            continue;
        }

        if (C != get<1>(centers[i])) {
            unchangedCenters = false;
            get<0>(centers[i]) = "OutofDataset";
            get<1>(centers[i]) = C;
            get<2>(centers[i]) = -1;
        }

    }

}

template<class inputData>
void
CurveClustering<inputData>::UpdateALaLoyd(InputGenericVector<inputData> const &curvesVector, bool &unchangedCenters) {
    unchangedCenters = true;
    vector<vector<Cell>> foo;
    for (unsigned int i = 0; i < k; ++i) {
        cout << "Calculating A la Loyd. Please wait.." << endl;
        double min_sum = numeric_limits<double>::max();
        int min_index = -1;
        for (unsigned long no_index1 = 0; no_index1 < clusters[i].indexes.size(); ++no_index1) {
            double temp_sum = 0;
            for (auto index2:clusters[i].indexes) {
                temp_sum += fast_distance_calculationC(clusters[i].indexes[no_index1], index2,
                                                       curvesVector.itemValues[clusters[i].indexes[no_index1]].second,
                                                       curvesVector.itemValues[index2].second);
            }
            if (min_sum > temp_sum) {
                min_sum = temp_sum;
                min_index = clusters[i].indexes[no_index1];
            }
        }

        //If empty cluster assign at random
        if (min_index == -1) {
            int randomIndex = generateNumberC(0, (int) curvesVector.itemValues.size() - 1);

            //Check if random value already exists in the centers
            while (true) {
                for (unsigned int cluster = 0; cluster < k; ++cluster) {
                    if (get<0>(centers[cluster]) == curvesVector.itemValues[randomIndex].first) {
                        randomIndex = generateNumberC(0, (int) curvesVector.itemValues.size() - 1);
                        continue;
                    }
                }
                break;
            }
            get<0>(centers[i]) = curvesVector.itemValues[randomIndex].first; //Assign randomly the ItemID of a point of pointVector
            get<1>(centers[i]) = curvesVector.itemValues[randomIndex].second; //Assign randomly the coordinates of a point of pointVector
            get<2>(centers[i]) = randomIndex;
            unchangedCenters = false;
            continue;
        }
        if (curvesVector.itemValues[min_index].second != get<1>(centers[i])) {
            unchangedCenters = false;
            get<0>(centers[i]) = curvesVector.itemValues[min_index].first;
            get<1>(centers[i]) = curvesVector.itemValues[min_index].second;
            get<2>(centers[i]) = min_index;
        }

    }
}


template<class inputData>
void CurveClustering<inputData>::Silhouette(InputGenericVector<inputData> &curvesVector) {
    double silhouete_total = 0;
    vector<vector<Cell>> foo;
    cout << "Silhouette: [";
    for (unsigned int i = 0; i < k; ++i) {
        double silhouete_of_cluster = 0;
        for (unsigned long item = 0; item < clusters[i].ItemIDs.size(); ++item) {
            double silhouete_of_item = 0;
            double a = 0;
            for (unsigned long item_in_cluster = 0; item_in_cluster < clusters[i].ItemIDs.size(); ++item_in_cluster) {
                if (item == item_in_cluster)
                    continue;
                a += fast_distance_calculationC(clusters[i].indexes[item], clusters[i].indexes[item_in_cluster],
                                                curvesVector.itemValues[clusters[i].indexes[item]].second,
                                                curvesVector.itemValues[clusters[i].indexes[item_in_cluster]].second);
            }
            if (clusters[i].ItemIDs.size() > 1)
                a /= clusters[i].ItemIDs.size() - 1;
            unsigned int second_best_cluster = i;
            double min = numeric_limits<double>::max();
            for (unsigned int cluster = 0; cluster < k; cluster++) {
                if (cluster == i || clusters[cluster].ItemIDs.empty())
                    continue;
                double temp = Dtw(curvesVector.itemValues[clusters[i].indexes[item]].second,
                                  clusters[cluster].centroidCoordinates, foo);
                if (min > temp) {
                    min = temp;
                    second_best_cluster = cluster;
                }
            }

            double b = 0;
            for (unsigned long item_in_cluster = 0;
                 item_in_cluster < clusters[second_best_cluster].ItemIDs.size(); ++item_in_cluster) {
                b += fast_distance_calculationC(clusters[i].indexes[item],
                                                clusters[second_best_cluster].indexes[item_in_cluster],
                                                curvesVector.itemValues[clusters[i].indexes[item]].second,
                                                curvesVector.itemValues[clusters[second_best_cluster].indexes[item_in_cluster]].second);

            }
            if (!clusters[second_best_cluster].ItemIDs.empty())
                b /= clusters[second_best_cluster].ItemIDs.size();
            silhouete_of_item = b - a;
            if (b > a)
                silhouete_of_item /= b;
            else
                silhouete_of_item /= a;
            silhouete_of_cluster += silhouete_of_item;
        }
        if (!clusters[i].ItemIDs.empty())
            silhouete_of_cluster /= clusters[i].ItemIDs.size();
        silhouete_total += silhouete_of_cluster;
        cout << silhouete_of_cluster << ",";
    }
    silhouete_total /= k;
    cout << " " << silhouete_total << "]" << endl;
}

template<class inputData>
void CurveClustering<inputData>::Printing(unsigned int const &whichInitialization, unsigned int const &whichAssignment,
                                          unsigned int const &whichUpdate, bool const &complete, double const &duration,
                                          InputGenericVector<inputData> &curvesVector, string const &outputFile) {
    cout << "Printing file, calculating Silhouette.." << endl;
    streambuf *psbuf, *backup;
    ofstream filestr;
    filestr.open(outputFile);

    backup = cout.rdbuf();     // back up cout's streambuf

    psbuf = filestr.rdbuf();        // get file's streambuf
    cout.rdbuf(psbuf);         // assign streambuf to cout
    cout << "Algorithm: I" << whichInitialization << "A" << whichAssignment << "U" << whichUpdate << endl;
    for (unsigned int i = 0; i < k; ++i) {
        cout << "CLUSTER-" << i + 1 << " {size: " << clusters[i].ItemIDs.size() << ", centroid: ";
        if ("OutofDataset" != clusters[i].centroid)
            cout << clusters[i].centroid;
        else {
            for (auto const &coord:clusters[i].centroidCoordinates)
                cout << "(" << coord.first << "," << coord.second << ")" << " ";
        }
        cout << "}" << endl;
    }

    cout << "Clustering_time: " << duration << endl;

    Silhouette(curvesVector);

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
    std::cout.rdbuf(backup);        // restore cout's original streambuf
    filestr.close();
}

template<class inputData>
CurveClustering<inputData>::CurveClustering(InputGenericVector<inputData> &curvesVector, unsigned int const &k_given,
                                            unsigned int const &maxSize, unsigned int const &minSize,
                                            unsigned int const &whichInitialization,
                                            unsigned int const &whichAssignment,
                                            unsigned int const &whichUpdate, unsigned int const &k_of_lsh,
                                            unsigned int const &L_grid, bool const &complete,
                                            string const &outputFile) {
    k = k_given;
    maxCurveSize = maxSize * 2;//Because its vector will consist of x1,y1,x2,y2,x3,y3 not in pairs
    minCurveSize = minSize;

    auto start = std::chrono::system_clock::now();
    cout << "Initializing.." << endl;
    if (whichInitialization == 1)
        InitializationSimplest(curvesVector);
    else
        InitializationKmeansPlusPlus(curvesVector);

    //In this preload we create the lsh structures and put all points in the hash tables in order to be queried by the centroids
    if (whichAssignment == 2)
        ReverseAssignmentPreload(curvesVector, k_of_lsh, L_grid, 2);

    bool unchangedCenters = false;
    int maxIterations = 3;
    //If the centers do not change we are done. We have the min objective function.
    while (!unchangedCenters && maxIterations-- > 0) {
        cout << "Working.." << endl;
        cout << "Assigning..." << endl;
        if (whichAssignment == 1)
            AssignmentSimplest(curvesVector);
        else
            ReverseAssignment(curvesVector);
        cout << "Updating Centers...." << endl;
        if (whichUpdate == 2)
            UpdateSimplest(curvesVector, unchangedCenters);
        else
            UpdateALaLoyd(curvesVector, unchangedCenters);

    }
    cout << "Clustering Done." << endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> clustering_duration = end - start;

    if (whichAssignment == 2)
        free(lsh);

    Printing(whichInitialization, whichAssignment, whichUpdate, complete, clustering_duration.count(), curvesVector,
             outputFile);
}

template
class CurveClustering<pair<double, double>>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.