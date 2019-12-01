#include "HashTables.h"

template<class inputData>
void HashTables<inputData>::insertHashtable(unsigned int whichHashTable, unsigned int const &g, int const &index) {
    hashTables[whichHashTable].insert(make_pair(g, index));
}

template<>
double HashTables<int>::manhattanDistance(vector<int> const &point, vector<int> const &query) {
    double distance = 0;
    for (unsigned long i = 0; i < point.size(); i++) {
        distance += abs(point[i] - query[i]);
    }
    return distance;
}

template<>
double HashTables<double>::manhattanDistance(vector<double> const &point, vector<double> const &query) {
    double distance = 0;
    for (unsigned long i = 0; i < point.size(); i++) {
        distance += abs(point[i] - query[i]);
    }
    return distance;
}

inline double minf(const double a, const double b, const double c) {
    double min;
    min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}

inline double Dtw(vector<pair<double, double>> const &p, vector<pair<double, double>> const &q) {
    double distance;
    double C[p.size()][q.size()];
    for (unsigned long i = 0; i < p.size(); i++) {
        for (unsigned long j = 0; j < q.size(); j++) {
            if (i == 0 && j == 0) {
                C[0][0] = sqrt((q[j].first - p[i].first) * (q[j].first - p[i].first) + (q[j].second - p[i].second) *
                                                                                       (q[j].second -
                                                                                        p[i].second));             //initialize first value with ||p-q||
                continue;
            }
            distance = sqrt((q[j].first - p[i].first) * (q[j].first - p[i].first) +
                            (q[j].second - p[i].second) * (q[j].second - p[i].second));   //Euclidean distance

            if (i > 0 && j > 0) {
                C[i][j] = minf(C[i - 1][j], C[i - 1][j - 1], C[i][j - 1]) + distance;
            } else if (i > 0 && j == 0) {
                C[i][0] = C[i - 1][0] + distance;
            } else if (j > 0 && i == 0) {
                C[0][j] = C[0][j - 1] + distance;
            }
        }
    }
    return C[p.size() - 1][q.size() - 1];
}

template<>
void
HashTables<int>::rangeSearch(tuple<string, vector<int>, int> &query, double radius,
                             vector<HashFunctions<int>> &hashFunctions, unsigned int const &k,
                             double const &w,
                             vector<struct cluster<int>> &clusters, int const &index,
                             unordered_map<int, pair<int, double>> &assignedItems,
                             InputGenericVector<int> const &pointsVector,
                             unordered_map<pair<int, int>, double, hash_pair> &calculatedDistances) {

    //int is the index of the point in pointsVector
    unordered_map<int, bool> alreadyInThisCluster;
    bool allBucketsEmpty = true;
    //for i from 1 to L do
    for (unsigned long i = 0; i < hashTables.size(); i++) {
        //cout<<"pame pali "<<query.first<<endl;
        unsigned int g = hashFunctions[i].gCalculator(get<1>(query), k, w);
        //for each item p in bucket gi(q) do
        for (auto p = hashTables[i].equal_range(g).first; p != hashTables[i].equal_range(g).second; p++) {
//            //if large number of retrieved items (e.g.>100L) then Break
//            if (hashTables[i].count(g) > 100 * hashTables.size())
//                break;

            allBucketsEmpty = false;
            //if dist(q,p) is within radius save the range neighbor
            double dist;
            //Check if we had already calculated this distance somewhere so we have it saved and don't need to recalculate it
            auto iterator_to_calculatedDistances = calculatedDistances.find(make_pair(p->second, get<2>(query)));
            if (iterator_to_calculatedDistances == calculatedDistances.end()) {
                double temp_dist = manhattanDistance(pointsVector.itemValues[p->second].second, get<1>(query));
                if (p->second != -1 && get<2>(query) != -1)
                    calculatedDistances[make_pair(p->second, get<2>(query))] = temp_dist;
                dist = temp_dist;
            } else {
                dist = iterator_to_calculatedDistances->second;
            }
            //cout<<"shmeio "<<get<0>(p->second)<<" apostasi " << dist<<endl;
            if (dist <= radius && alreadyInThisCluster.find(p->second) == alreadyInThisCluster.end()) {

                //If the point is not assigned,save the ItemID and its index in pointsVector
                //Assign it and save the distance for future comparisons
                //cout<<"bazw to shmeio "<<get<0>(p->second)<<" me assignment "<< get<3>(p->second).first<<" sto "<<index<<" apo to table "<<i<<" me g "<<g<<" kai radius "<<radius<<endl;
                auto iterator_to_assignedItems = assignedItems.find(p->second);
                if (get<0>(iterator_to_assignedItems->second) == -1) {
                    clusters[index].ItemIDs.push_back(pointsVector.itemValues[p->second].first);
                    clusters[index].indexes.push_back(p->second);
                    get<0>(iterator_to_assignedItems->second) = index;
                    get<1>(iterator_to_assignedItems->second) = dist;

                } else {
                    //If the point is assigned, check if current distance is less than the cluster which is assigned and if yes, reassign it
                    if (dist < get<1>(iterator_to_assignedItems->second)) {
                        clusters[index].ItemIDs.push_back(pointsVector.itemValues[p->second].first);
                        clusters[index].indexes.push_back(p->second);

                        //Pop from already assigned cluster
                        for (unsigned long id = 0;
                             id < clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs.size(); ++id) {
                            if (clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs[id] ==
                                pointsVector.itemValues[p->second].first) {
                                clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs.erase(
                                        clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs.begin() + id);
                                clusters[get<0>(iterator_to_assignedItems->second)].indexes.erase(
                                        clusters[get<0>(iterator_to_assignedItems->second)].indexes.begin() + id);
                            }
                        }

                        get<0>(iterator_to_assignedItems->second) = index;
                        get<1>(iterator_to_assignedItems->second) = dist;
                    }
                }
                alreadyInThisCluster[p->second] = true;

            }

        }
    }

    if (allBucketsEmpty) {
        cout << "THE IMPOSSIBLE HAPPENED: ALL BUCKETS ARE EMPTY!!!" << endl;
    }
}

template<>
void
HashTables<double>::rangeSearch(tuple<string, vector<double>, int> &query, double radius,
                                vector<HashFunctions<double>> &hashFunctions, unsigned int const &k,
                                double const &w,
                                vector<struct cluster<double>> &clusters, int const &index,
                                unordered_map<int, pair<int, double>> &assignedItems,
                                InputGenericVector<double> const &pointsVector,
                                unordered_map<pair<int, int>, double, hash_pair> &calculatedDistances) {

    //int is the index of the point in pointsVector
    unordered_map<int, bool> alreadyInThisCluster;
    bool allBucketsEmpty = true;
    //for i from 1 to L do
    for (unsigned long i = 0; i < hashTables.size(); i++) {
        //cout<<"pame pali "<<query.first<<endl;
        unsigned int g = hashFunctions[i].gCalculator(get<1>(query), k, w);
        //for each item p in bucket gi(q) do
        for (auto p = hashTables[i].equal_range(g).first; p != hashTables[i].equal_range(g).second; p++) {
//            //if large number of retrieved items (e.g.>100L) then Break
//            if (hashTables[i].count(g) > 100 * hashTables.size())
//                break;

            allBucketsEmpty = false;
            //if dist(q,p) is within radius save the range neighbor
            double dist;
            //Check if we had already calculated this distance somewhere so we have it saved and don't need to recalculate it
            auto iterator_to_calculatedDistances = calculatedDistances.find(make_pair(p->second, get<2>(query)));
            if (iterator_to_calculatedDistances == calculatedDistances.end()) {
                double temp_dist = manhattanDistance(pointsVector.itemValues[p->second].second, get<1>(query));
                if (p->second != -1 && get<2>(query) != -1)
                    calculatedDistances[make_pair(p->second, get<2>(query))] = temp_dist;
                dist = temp_dist;
            } else {
                dist = iterator_to_calculatedDistances->second;
            }


            //cout<<"shmeio "<<get<0>(p->second)<<" apostasi " << dist<<endl;
            if (dist <= radius && alreadyInThisCluster.find(p->second) == alreadyInThisCluster.end()) {

                //If the point is not assigned,save the ItemID and its index in pointsVector
                //Assign it and save the distance for future comparisons
                //cout<<"bazw to shmeio "<<get<0>(p->second)<<" me assignment "<< get<3>(p->second).first<<" sto "<<index<<" apo to table "<<i<<" me g "<<g<<" kai radius "<<radius<<endl;
                auto iterator_to_assignedItems = assignedItems.find(p->second);
                if (get<0>(iterator_to_assignedItems->second) == -1) {
                    clusters[index].ItemIDs.push_back(pointsVector.itemValues[p->second].first);
                    clusters[index].indexes.push_back(p->second);
                    get<0>(iterator_to_assignedItems->second) = index;
                    get<1>(iterator_to_assignedItems->second) = dist;

                } else {
                    //If the point is assigned, check if current distance is less than the cluster which is assigned and if yes, reassign it
                    if (dist < get<1>(iterator_to_assignedItems->second)) {
                        clusters[index].ItemIDs.push_back(pointsVector.itemValues[p->second].first);
                        clusters[index].indexes.push_back(p->second);

                        //Pop from already assigned cluster
                        for (unsigned long id = 0;
                             id < clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs.size(); ++id) {
                            if (clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs[id] ==
                                pointsVector.itemValues[p->second].first) {
                                clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs.erase(
                                        clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs.begin() + id);
                                clusters[get<0>(iterator_to_assignedItems->second)].indexes.erase(
                                        clusters[get<0>(iterator_to_assignedItems->second)].indexes.begin() + id);
                            }
                        }

                        get<0>(iterator_to_assignedItems->second) = index;
                        get<1>(iterator_to_assignedItems->second) = dist;
                    }
                }
                alreadyInThisCluster[p->second] = true;

            }

        }
    }

    if (allBucketsEmpty) {
        cout << "THE IMPOSSIBLE HAPPENED: ALL BUCKETS ARE EMPTY!!!" << endl;
    }
}


template<>
void
HashTables<pair<double, double>>::rangeSearch(tuple<string, vector<pair<double, double>>, int> &query, double radius,
                                              unsigned int const &g,
                                              vector<struct cluster<pair<double, double>>> &clusters, int const &index,
                                              unordered_map<int, pair<int, double>> &assignedItems,
                                              InputGenericVector<pair<double, double>> const &curvesVector,
                                              unordered_map<pair<int, int>, double, hash_pair> &calculatedDistances) {
    //int is the index of the curve in curvesVector
    unordered_map<int, bool> alreadyInThisCluster;
    bool allBucketsEmpty = true;
    //for each item p in bucket gi(q) do
    for (auto p = hashTables[0].equal_range(g).first; p != hashTables[0].equal_range(g).second; p++) {

        allBucketsEmpty = false;
        //if dist(q,p) is within radius save the range neighbor
        double dist;
        //Check if we had already calculated this distance somewhere so we have it saved and don't need to recalculate it
        auto iterator_to_calculatedDistances = calculatedDistances.find(make_pair(p->second, get<2>(query)));
        if (iterator_to_calculatedDistances == calculatedDistances.end()) {
            double temp_dist = Dtw(curvesVector.itemValues[p->second].second, get<1>(query));
            if (p->second != -1 && get<2>(query) != -1)
                calculatedDistances[make_pair(p->second, get<2>(query))] = temp_dist;
            dist = temp_dist;
        } else {
            dist = iterator_to_calculatedDistances->second;
        }

        if (dist <= radius && alreadyInThisCluster.find(p->second) == alreadyInThisCluster.end()) {

            //If the curve is not assigned,save the ItemID and its index
            //Assign it and save the distance for future comparisons
            auto iterator_to_assignedItems = assignedItems.find(p->second);
            if (get<0>(iterator_to_assignedItems->second) == -1) {
                clusters[index].ItemIDs.push_back(curvesVector.itemValues[p->second].first);
                clusters[index].indexes.push_back(p->second);
                get<0>(iterator_to_assignedItems->second) = index;
                get<1>(iterator_to_assignedItems->second) = dist;

            } else {
                //If the point is assigned, check if current distance is less than the cluster which is assigned and if yes, reassign it
                if (dist < get<1>(iterator_to_assignedItems->second)) {
                    clusters[index].ItemIDs.push_back(curvesVector.itemValues[p->second].first);
                    clusters[index].indexes.push_back(p->second);

                    //Pop from already assigned cluster
                    for (unsigned long id = 0;
                         id < clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs.size(); ++id) {
                        if (clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs[id] ==
                            curvesVector.itemValues[p->second].first) {
                            clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs.erase(
                                    clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs.begin() + id);
                            clusters[get<0>(iterator_to_assignedItems->second)].indexes.erase(
                                    clusters[get<0>(iterator_to_assignedItems->second)].indexes.begin() + id);
                        }
                    }

                    get<0>(iterator_to_assignedItems->second) = index;
                    get<1>(iterator_to_assignedItems->second) = dist;
                }
            }
            alreadyInThisCluster[p->second] = true;

        }

    }

    if (allBucketsEmpty) {
        cout << "THE IMPOSSIBLE HAPPENED: ALL BUCKETS ARE EMPTY!!!" << endl;
    }
}

template
class HashTables<int>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.
template
class HashTables<double>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.
template
class HashTables<pair<double, double>>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.