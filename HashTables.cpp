#include "HashTables.h"

template<class inputData>
void HashTables<inputData>::insertHashtable(unsigned int whichHashTable, unsigned int const &g,
                                            pair<string, vector<inputData >> &point, int const &index) {
    hashTables[whichHashTable].insert(make_pair(g, make_tuple(point.first, point.second, index)));
}

template<class inputData>
inline double HashTables<inputData>::manhattanDistance(vector<inputData> const &point, vector<inputData> const &query) {
    double distance = 0;
    for (unsigned long i = 0; i < point.size(); i++) {
        distance += abs(point[i] - query[i]);
    }
    return distance;
}


template<class inputData>
void
HashTables<inputData>::rangeSearch(pair<string, vector<inputData >> &query, double radius,
                                   vector<HashFunctions<inputData>> hashFunctions, unsigned int const &k,
                                   double const &w,
                                   vector<struct cluster<inputData>> &clusters, int const &index,
                                   unordered_map<string, tuple<int, double, int>> &assignedItems) {

    unordered_map<string, bool> alreadyInThisCluster;
    bool allBucketsEmpty = true;
    //for i from 1 to L do
    for (unsigned long i = 0; i < hashTables.size(); i++) {
        //cout<<"pame pali "<<query.first<<endl;
        unsigned int g = hashFunctions[i].gCalculator(query.second, k, w);
        //for each item p in bucket gi(q) do
        for (auto p = hashTables[i].equal_range(g).first; p != hashTables[i].equal_range(g).second; p++) {
//            //if large number of retrieved items (e.g.>100L) then Break
//            if (hashTables[i].count(g) > 100 * hashTables.size())
//                break;

            allBucketsEmpty = false;
            //if dist(q,p) is within radius save the range neighbor
            double dist = manhattanDistance(get<1>(p->second), query.second);
            //cout<<"shmeio "<<get<0>(p->second)<<" apostasi " << dist<<endl;
            if (dist <= radius && alreadyInThisCluster.find(get<0>(p->second)) == alreadyInThisCluster.end()) {

                //If the point is not assigned,save the ItemID and its index in pointsVector
                //Assign it and save the distance for future comparisons
                //cout<<"bazw to shmeio "<<get<0>(p->second)<<" me assignment "<< get<3>(p->second).first<<" sto "<<index<<" apo to table "<<i<<" me g "<<g<<" kai radius "<<radius<<endl;
                auto iterator_to_assignedItems = assignedItems.find(get<0>(p->second));
                if (get<0>(iterator_to_assignedItems->second) == -1) {
                    clusters[index].ItemIDs.push_back(get<0>(p->second));
                    clusters[index].indexes.push_back(get<2>(p->second));
                    get<0>(iterator_to_assignedItems->second) = index;
                    get<1>(iterator_to_assignedItems->second) = dist;

                } else {
                    //If the point is assigned, check if current distance is less than the cluster which is assigned and if yes, reassign it
                    if (dist < get<1>(iterator_to_assignedItems->second)) {
                        clusters[index].ItemIDs.push_back(get<0>(p->second));
                        clusters[index].indexes.push_back(get<2>(p->second));

                        //Pop from already assigned cluster
                        for (unsigned long id = 0;
                             id < clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs.size(); ++id) {
                            if (clusters[get<0>(iterator_to_assignedItems->second)].ItemIDs[id] == get<0>(p->second)) {
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
                alreadyInThisCluster[get<0>(p->second)] = true;

            }

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