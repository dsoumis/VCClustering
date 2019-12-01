#include <iostream>
#include "InputImplementatition.h"
#include "InitializationCurves.h"


int main(int argc, char **argv) {

    int centersNumber = 3;
    unsigned int maxCurveSize = 0, minCurveSize = 4294967295;

    string inputFile=string(argv[1]);

    ifstream file;

    file.open(inputFile);
    string firstLine;
    getline(file, firstLine);  //read first line
    file.close();



    InputGenericVector<pair<double, double>> curvesVector(inputFile, maxCurveSize, minCurveSize, false);
    curvesVector.printVector();
    vector<int> centers;                            //vector of the places of curvesVector as centers
    chooseRandomCenter(curvesVector, centers);

    for(int i=0;i <centersNumber; i++){
        createNewCenter(curvesVector, centers);
    }
    for(auto& cen:centers){
        cout<<"- "<<cen<<endl;
    }
    


    return 0;
}