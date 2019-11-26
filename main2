#include <iostream>
#include "InputImplementatition.h"
#include "Initialization.h"


int main() {

    int centersNumber =3;
    InputGenericVector<double> pointsVector("C:\\Users\\Yiouli\\Desktop\\Ex2_Datasets\\DataVectors_5_500x100.csv");
    pointsVector.printVector();

    vector<int> centers;                            //vector of the places of pointsVector as centers
    chooseRandomCenter(pointsVector, centers);
    for(int i=0;i <centersNumber; i++){
        createNewCenter(pointsVector, centers);
    }

    for(auto& cen:centers){
        cout<<"- "<<cen<<endl;
    }



    return 0;
}
