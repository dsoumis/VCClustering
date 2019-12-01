#include "Dtw.h"


template <>
double calculateC( vector<vector<double>> &C, vector<pair<double,double>> &p,vector<pair<double,double>> &q){
    double distance;
    for(int i=0; i<p.size(); i++) {
        for(int j = 0; j<q.size(); j++) {

            distance = calculateDistance(p[i].first, p[i].second, q[j].first, q[j].second);    //Euclidean distance

            if (i > 0 && j > 0) {
                C[i][j] = min(C[i - 1][j], C[i - 1][j - 1], C[i][j-1]) + distance;
            } else if (i > 0) {
                C[i][j] = C[i-1][0] + distance;
            } else if (j > 0) {
                C[i][j] =  C[0][j-1] + distance;
            }
        }
    }
    return C[p.size()-1][q.size()-1];
}


template <>
double Dtw(vector<pair<double,double>> &p,vector<pair<double,double>> &q){
    vector<vector<double>> C;      //2-d vector

    C.resize(p.size());
    for(int i=0; i<p.size(); i++){
        C[i].resize(q.size());
    }
    C[0][0] = calculateDistance(p[0].first, p[0].second, q[0].first, q[0].second);              //initialize first value with ||p-q||

    return calculateC(C, p, q);
}


double calculateDistance(const double x0, const double y0, const double x1, const double y1) {
    double x,y;
    x = (x1 - x0)*(x1 - x0);
    y = (y1 - y0)*(y1 - y0);
    return sqrt(x + y);
}

double min(const double a,const double b,const double c) {
    double min;
    min=a;
    if(b<min) min=b;
    if(c<min) min=c;
    return min;
}
