#include <limits>
#include <sstream>
#include "InputImplementation.h"

template<class inputData>
void InputGenericVector<inputData>::push(inputData const &value) {
    temp.push_back(value);
}

template<>
void InputGenericVector<double>::save() {
    itemValues.emplace_back(make_pair(itemID, temp));
    temp.clear();
}

template<>
void InputGenericVector<int>::save() {
    itemValues.emplace_back(make_pair(itemID, temp));
    temp.clear();
}

template<>
void InputGenericVector<pair<double, double>>::save() {
    if (temp != previous)
        itemValues.emplace_back(make_pair(itemID, temp));
    previous.clear();
    previous.shrink_to_fit();
    previous.resize(temp.size());
    previous = temp;
    temp.clear();
}


template<class inputData>
void InputGenericVector<inputData>::printVector() {
    for (unsigned long i = 0; i < itemValues.size(); i++) {
        //cout<<itemValues[i].second.size()<<endl;
        cout << itemValues[i].first << endl;
        for (unsigned long j = 0; j < itemValues[i].second.size(); j++) {
            cout << itemValues[i].second.at(j) << " ";
        }
        cout << endl;
    }
}

template<>
void InputGenericVector<pair<double, double>>::maxCoordFinder(double &max) {
    for (auto const &item:itemValues) {
        for (auto const &item_ind:item.second) {
            if (max < item_ind.first)
                max = item_ind.first;
            if (max < item_ind.second)
                max = item_ind.second;
        }
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

template<>
void InputGenericVector<pair<double, double>>::constructorFunction(InputGenericVector<pair<double, double>> &vector,
                                                                   string const &value) {
    stringstream stream(value);
    pair<double, double> vector_value;
    string temp_str;
    getline(stream, temp_str, ' ');
    vector_value.first = stod(temp_str);
    getline(stream, temp_str, ' ');
    vector_value.second = stod(temp_str);
    vector.push(vector_value);
}

template<class inputData>
InputGenericVector<inputData>::InputGenericVector(string const &path) { //For input file
    //The input file
    ifstream inputFile;
    try {
        inputFile.open(path);
        if (!inputFile.is_open())
            cout << "Can't open file" << endl;
        string firstLine;
        getline(inputFile, firstLine);  //read first line
        while (!inputFile.eof()) {
            string sLine;
            //Read line by line
            getline(inputFile, sLine);
            if (sLine.length() == 0)//Break if it's last line.
                break;


            bool itemIDflag = true;
            string word;
            for (auto &c:sLine) {
                //Push every value delimited by white space
                if (iswspace(c)) {
                    if (!itemIDflag) { //If it is not the item's id
                        //this->push(stoi(sLine.substr(0, pos)));
                        constructorFunction(*this, word);           //push the created string
                    } else {
                        itemID = word;
                        itemIDflag = false;
                    }
                    word = "";
                } else {
                    word += c;
                }

            }
            //for the last word of line
            if (!itemIDflag) { //If it is not the item's id
                //this->push(stoi(sLine.substr(0, pos)));
                constructorFunction(*this, word);
            } else {
                itemID = word;
            }
            //Save the pushed values
            this->save();
        }

        inputFile.close();
    } catch (const char *msg) {
        cerr << msg << endl;
    }
}


template<>
void InputGenericVector<pair<double, double>>::printVector() {
    for (auto const &item: itemValues) {
        //cout<<itemValues[i].second.size()<<endl;
        cout << item.first << endl;
        cout.precision(numeric_limits<double>::max_digits10);
        for (auto it:item.second) {
            cout << "(" << it.first << "," << it.second << ") ";
        }
        cout << endl;
    }
}

template<>
InputGenericVector<pair<double, double>>::InputGenericVector(string const &path, unsigned int &maxCurveSize,
                                                             unsigned int &minCurveSize,
                                                             bool const &input) { //Specific for trajectories dataset. Wont work with other files due to line variable.
    //The input file
    previous.emplace_back(make_pair(0, 0));
    ifstream inputFile;
    try {
        inputFile.open(path);
        if (!inputFile.is_open())
            cout << "Can't open file" << endl;

        unsigned int max_m = 0; //Max size of all grids
        unsigned int min_m = 4294967295;//Min size of all grids
        string firstLine;
        getline(inputFile, firstLine);  //read first line


        while (!inputFile.eof()) {
            string sLine;
            //Read line by line
            getline(inputFile, sLine);

            if (sLine.length() == 0)//Break if it's last line=empty line.
                break;
            size_t pos = 0;

            while ((pos = sLine.find('\t')) != string::npos) {
                itemID = sLine.substr(0, pos);

                sLine.erase(0, pos + 1);
                pos = sLine.find('\t');
                unsigned int curve_size = (unsigned int) stoi(sLine.substr(0, pos));
                if (curve_size > max_m)
                    max_m = curve_size;
                if (curve_size < min_m)
                    min_m = curve_size;
                sLine.erase(0, pos + 1);
                for (unsigned int i = 0; i < curve_size; i++) {
                    pos = sLine.find(',');
                    string temp_str = sLine.substr(0, pos);
                    temp_str.erase(0, 1); //Remove first character of string which is (

                    sLine.erase(0, pos + 1);
                    pos = sLine.find(')');
                    temp_str += sLine.substr(0, pos);
                    //cout<<"to temp pou stelnw "<<temp<<endl;
                    constructorFunction(*this, temp_str);
                    sLine.erase(0, pos + 2);
                }
            }
            //Save the pushed values
            this->save();
        }

        inputFile.close();
        if (max_m > maxCurveSize)
            maxCurveSize = max_m;
        if (min_m < minCurveSize)
            minCurveSize = min_m;
    } catch (const char *msg) {
        cerr << msg << endl;
    }
}

template
class InputGenericVector<int>; //In order to not fail the compile as the compiler wants to see the data that the templated class will have.
template
class InputGenericVector<double>;

template
class InputGenericVector<pair<double, double>>;