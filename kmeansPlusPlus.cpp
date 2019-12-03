#include "kmeansPlusPlus.h"

template<>
void chooseRandomCenter(InputGenericVector<double> const &vector, ::vector<int> &centers) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(0, (int) vector.itemValues.size() - 1);

    centers.push_back(dis(gen));  //choose the first center
}

template<>
void chooseRandomCenter(InputGenericVector<int> const &vector, ::vector<int> &centers) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(0, (int) vector.itemValues.size() - 1);

    centers.push_back(dis(gen));  //choose the first center
}

template<>
void chooseRandomCenter(InputGenericVector<pair<double, double>> const &vector, ::vector<int> &centers) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(0, (int) vector.itemValues.size() - 1);

    centers.push_back(dis(gen));  //choose the first center
}

double chooseRandomX(vector<double> &P) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, P[(int) P.size() - 1]);  //random number between 0 and P[n-t]
    double x = dis(gen);
    return x;

}

int binarySearch(vector<double> &x, double my_number) {
    int l = 0;
    int r = (int) x.size() - 1;
    int closer = -1;
    while (true) {      //binary search
        int m = (l + r) / 2;
        if (x[m] == my_number) {
            closer = m;
            break;
        } else if (r - l <= 1) {
            break;
        } else if (x[m] < my_number) {
            l = m;
        } else if (x[m] > my_number) {
            r = m;
        }
    }
    if (closer != -1) {
        return closer;      //if the search finds the exact given number
    } else {
        return r;       //else return the max of the last 2 numbers
    }
}

void calculatePartialSums(vector<double> &P, vector<double> &D) {
    for (unsigned long i = 0; i < P.size(); i++) {
        if (i == 0) {                     //if its the first iteration
            P[i] = D[i] * D[i];           //calculate partial sum
        } else {
            P[i] = P[i - 1] + D[i] * D[i];       //calculate partial sum
        }
//        cout.precision(17);
//        cout<<P[i]<<" ";
    }
//    cout<<endl;

}

template<>
void countDist(InputGenericVector<double> const &pointsVec, ::vector<int> &centers, vector<double> &D) {


    for (unsigned long i = 0;
         i < pointsVec.itemValues.size(); i++) {       //for every point find the distance to the closest center
        vector<int>::iterator it;
        it = std::find(centers.begin(), centers.end(), i);
        if (it != centers.end()) {    //skip the actual center points
            continue;
        }

        pair<double, int> minDistance = findNearestCenter(pointsVec.itemValues[i].second, centers, pointsVec, i);
        D.push_back(minDistance.first);
    }
//    cout<<endl;
//    for(auto& x:D){
//        cout<<x<<" ";
//    }
//    cout<<endl;

}

template<>
void countDist(InputGenericVector<pair<double, double>> const &pointsVec, ::vector<int> &centers, vector<double> &D) {

    for (unsigned long i = 0;
         i < pointsVec.itemValues.size(); i++) {       //for every point find the distance to the closest center
        vector<int>::iterator it;
        it = std::find(centers.begin(), centers.end(), i);
        if (it != centers.end()) {    //skip the actual center points
            continue;
        }

        pair<double, int> minDistance = findNearestCenter(pointsVec.itemValues[i].second, centers, pointsVec, i);
        D.push_back(minDistance.first);
    }
//    cout<<endl;
//    for(auto& x:D){
//        cout<<x<<" ";
//    }
//    cout<<endl;

}

template<>
void countDist(InputGenericVector<int> const &pointsVec, ::vector<int> &centers, vector<double> &D) {


    for (unsigned long i = 0;
         i < pointsVec.itemValues.size(); i++) {       //for every point find the distance to the closest center
        vector<int>::iterator it;
        it = std::find(centers.begin(), centers.end(), i);
        if (it != centers.end()) {    //skip the actual center points
            continue;
        }

        pair<double, int> minDistance = findNearestCenter(pointsVec.itemValues[i].second, centers, pointsVec, i);
        D.push_back(minDistance.first);
    }
//    cout<<endl;
//    for(auto& x:D){
//        cout<<x<<" ";
//    }
//    cout<<endl;

}

template<>
void createNewCenter(InputGenericVector<double> const &pointsVec, vector<int> &centers) {
    vector<double> D;
    countDist(pointsVec, centers, D);     //Calculate distances D(i)

    vector<double> P;
    P.resize(D.size());
    calculatePartialSums(P, D);         //calculate the partial sums and store them at vector P
    double x = chooseRandomX(P);       //choose a random double x
    int r = binarySearch(P, x);      //choose r
    centers.push_back(r);           //add center
}

template<>
void createNewCenter(InputGenericVector<int> const &pointsVec, vector<int> &centers) {
    vector<double> D;
    countDist(pointsVec, centers, D);     //Calculate distances D(i)

    vector<double> P;
    P.resize(D.size());
    calculatePartialSums(P, D);         //calculate the partial sums and store them at vector P
    double x = chooseRandomX(P);       //choose a random double x
    int r = binarySearch(P, x);      //choose r
    centers.push_back(r);           //add center
}

template<class inputData>
double manhattanDistance(::vector<inputData> const &center, ::vector<inputData> const &point) {
    double distance = 0;

    for (unsigned long i = 0; i < center.size(); i++) {
        distance += abs(center[i] - point[i]);
    }
    return distance;

}

template<>
double manhattanDistance(::vector<pair<double, double >> const &center, ::vector<pair<double, double >> const &point) {

}

pair<double, int> findNearestCenter(::vector<int> const &item, ::vector<int> &centers,
                                    InputGenericVector<int> const &pointsVec,
                                    unsigned long &i) {

    double min = 0;
    int minPos = 0;
    int flag = 0;

    for (auto &pos:centers) {
        if (i == pos)    //if its the center
            continue;
        double distance = manhattanDistance(pointsVec.itemValues[pos].second, item);
        if (flag == 0) {//initialize min
            min = distance;
            minPos = 0;
            flag = 1;
            continue;
        }
        if (distance < min) {
            min = distance;
            minPos = pos;
        }
    }

    return make_pair(min, minPos);   //return the nearest distance and center
}

pair<double, int> findNearestCenter(::vector<double> const &item, ::vector<int> &centers,
                                    InputGenericVector<double> const &pointsVec,
                                    unsigned long &i) {

    double min = 0;
    int minPos = 0;
    int flag = 0;

    for (auto &pos:centers) {
        if (i == pos)    //if its the center
            continue;
        double distance = manhattanDistance(pointsVec.itemValues[pos].second, item);
        if (flag == 0) {//initialize min
            min = distance;
            minPos = 0;
            flag = 1;
            continue;
        }
        if (distance < min) {
            min = distance;
            minPos = pos;
        }
    }

    return make_pair(min, minPos);   //return the nearest distance and center
}

pair<double, int> findNearestCenter(::vector<pair<double, double>> const &item, ::vector<int> &centers,
                                    InputGenericVector<pair<double, double>> const &curvesVec, unsigned long &i) {

    double min = 0;
    int minPos = 0;
    int flag = 0;

    for (auto &pos:centers) {
        if (i == pos)    //if its the center
            continue;
        vector<vector<Cell>> foo;         //2-d vector
        double distance = Dtw(curvesVec.itemValues[pos].second, item, foo);
        if (flag == 0) {//initialize min
            min = distance;
            minPos = 0;
            flag = 1;
            continue;
        }
        if (distance < min) {
            min = distance;
            minPos = pos;
        }
    }

    return make_pair(min, minPos);   //return the nearest distance and center
}

template<>
void createNewCenter(InputGenericVector<pair<double, double> > const &pointsVec, vector<int> &centers) {
    vector<double> D;
    countDist(pointsVec, centers, D);     //Calculate distances D(i)

    vector<double> P;
    P.resize(D.size());
    calculatePartialSums(P, D);         //calculate the partial sums and store them at vector P
    double x = chooseRandomX(P);       //choose a random double x
    int r = binarySearch(P, x);      //choose r
    centers.push_back(r);           //add center
}

template
class InputGenericVector<int>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.
template
class InputGenericVector<double>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.