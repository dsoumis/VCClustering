#include "Dtw.h"
#include "InputImplementation.h"
#include "ProjectionArray.h"
#define d 2
#define e 0.5


int main() {
    vector<pair<double,double>> p;
    vector<pair<double,double>> q;
    p.resize(4);
    q.resize(4);
    double x = 1.2, y = 3.5;
    for(int i = 0; i < 4; i++){
        p[i].first = x + i;
        p[i].second = x*i;
        q[i].first = y + i;
        q[i].second = y*i;

    }


    vector<vector<Cell>> C;         //2-d vector
    double dist = Dtw(p, q, C);
    cout<<"Distance: "<<C[p.size()-1][q.size()-1].value<<endl;
    vector<pair<int,int>> path = printPath(C);

    for(auto& num : path ){ //print path
        cout<<"("<<num.first<<","<<num.second<<")"<<"\t";
    }
    cout<<endl;
    return 0;
}


