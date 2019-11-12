#include "InputImplementation.h"

template<class inputData>
void InputGenericVector<inputData>::push(inputData const &value) {
    temp.push_back(value);
}

template<class inputData>
void InputGenericVector<inputData>::save() {
    itemValues.push_back(make_pair(itemID, temp));
    temp.clear();
}

template<class inputData>
void InputGenericVector<inputData>::printVector() {
    for (unsigned int i = 0; i < itemValues.size(); i++) {
        //cout<<itemValues[i].second.size()<<endl;
        cout << itemValues[i].first << endl;
        for (unsigned int j = 0; j < itemValues[i].second.size(); j++) {
            cout << itemValues[i].second.at(j) << " ";
        }
        cout << endl;
    }
}

template<>
void InputGenericVector<int>::constructorFunction(InputGenericVector<int> &vector, string const &value) {
    vector.push(stoi(value));
}

template<>
void InputGenericVector<double>::constructorFunction(InputGenericVector<double> &vector, string const &value) {
    vector.push(stod(value));
}

template<class inputData>
InputGenericVector<inputData>::InputGenericVector(string path) { //For input file
    //The input file
    ifstream inputFile;
    try {
        inputFile.open(path);
        if (!inputFile.is_open())
            cout << "Can't open file" << endl;
        while (!inputFile.eof()) {
            string sLine;
            //Read line by line
            getline(inputFile, sLine);
            if (sLine.length() == 0)//Break if it's last line.
                break;
            size_t pos = 0;

            bool itemIDflag = true;
            while ((pos = sLine.find(' ')) != string::npos) {
                //Push every value delimited by space
                if (!itemIDflag) //If it is not the item's id
                    //this->push(stoi(sLine.substr(0, pos)));
                    constructorFunction(*this, sLine.substr(0, pos));
                else {
                    itemID = sLine.substr(0, pos);
                    itemIDflag = false;
                }
                sLine.erase(0, pos + 1);
            }
            //Save the pushed values
            this->save();
        }

        inputFile.close();
    } catch (const char *msg) {
        cerr << msg << endl;
    }
}

template<class inputData>
InputGenericVector<inputData>::InputGenericVector(string path, double &radius) { //For query file
    //The input file
    ifstream inputFile;
    try {
        inputFile.open(path);
        if (!inputFile.is_open())
            cout << "Can't open file" << endl;
        while (!inputFile.eof()) {
            string sLine;
            //Read line by line
            getline(inputFile, sLine);
            if (sLine.length() == 0)//Break if it's last line.
                break;
            size_t pos = 0;
            if (sLine.find("Radius:") != string::npos) {
                pos = sLine.find(' ');
                sLine.erase(0, pos + 1);
                pos = sLine.find(' ');
                radius = stod(sLine.substr(0, pos));
                continue;
            }


            bool itemIDflag = true;
            while ((pos = sLine.find(' ')) != string::npos) {
                //Push every value delimited by space
                if (!itemIDflag) //If it is not the item's id
                    //this->push(stoi(sLine.substr(0, pos)));
                    constructorFunction(*this, sLine.substr(0, pos));
                else {
                    itemID = sLine.substr(0, pos);
                    itemIDflag = false;
                }
                sLine.erase(0, pos + 1);
            }
            //Save the pushed values
            this->save();
        }

        inputFile.close();
    } catch (const char *msg) {
        cerr << msg << endl;
    }
}


template
class InputGenericVector<int>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.
template
class InputGenericVector<double>;