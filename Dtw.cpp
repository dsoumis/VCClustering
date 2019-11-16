#include "Dtw.h"

template <>
double calculateC( vector<vector<Cell>> &C, vector<pair<double,double>> &p,vector<pair<double,double>> &q){
    double distance;
    for(int i=0; i<p.size(); i++) {
        for(int j = 0; j<q.size(); j++) {

            distance = calculateDistance(p[i].first, p[i].second, q[j].first, q[j].second);    //Euclidean distance

            if (i > 0 && j > 0) {
                pair<double,int> m = min(C[i - 1][j].value, C[i - 1][j - 1].value, C[i][j-1].value);

                C[i][j].value = m.first + distance ;
                C[i][j].i = i;                      //keep the place in the array
                C[i][j].j = j;
                if(m.second == 1){                      //keep previous for path
                    C[i][j].previous = &C[i - 1][j];
                }else if(m.second == 2){
                    C[i][j].previous = &C[i - 1][j - 1];
                }else if(m.second == 3){
                    C[i][j].previous = &C[i][j-1];
                }
            } else if (i > 0) {
                C[i][j].value = C[i-1][0].value + distance;
                C[i][j].i = i;
                C[i][j].j = j;
                C[i][j].previous = &C[i - 1][0];
            } else if (j > 0) {
                C[i][j].value =  C[0][j-1].value + distance;
                C[i][j].i = i;
                C[i][j].j = j;
                C[i][j].previous = &C[0][j - 1];
            }
        }
    }
    return C[p.size()-1][q.size()-1].value;

}
vector<pair<int,int>> printPath(vector<vector<Cell>> &C){
    int i = C.size(), j = C[0].size() ;
    Cell *temp = &C[i-1][j-1];          //start from the last value
    vector<pair<int,int>> path;
    while(temp!=nullptr){
        path.emplace_back(make_pair(temp->i,temp->j));
        temp = temp->previous;
    }
    reverse(path.begin(),path.end());       //reverse the vector so the values are properly positioned

    return path;

}

template <>
double Dtw(vector<pair<double,double>> &p,vector<pair<double,double>> &q, vector<vector<Cell>> &C){

    C.resize(p.size());
    for(int i=0; i<p.size(); i++){
        C[i].resize(q.size());
    }
    C[0][0].value = calculateDistance(p[0].first, p[0].second, q[0].first, q[0].second);              //initialize first value with ||p-q||

    return calculateC(C, p, q);
}


double calculateDistance(const double x0, const double y0, const double x1, const double y1) {
    double x,y;
    x = (x1 - x0)*(x1 - x0);
    y = (y1 - y0)*(y1 - y0);
    return sqrt(x + y);
}

pair<double,int> min(const double a,const double b,const double c) {
    double min;
    int num = 1;
    min = a;
    if(b<min){
        num = 2;
        min = b;
    }
    if(c<min) {
        num = 3;
        min=c;
    }

    return make_pair(min,num);;
}
