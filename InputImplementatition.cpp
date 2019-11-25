#include "InputImplementatition.h"

template <class inputData>
void InputGenericVector<inputData>::push (inputData const& value) {
    temp.push_back(value);
}
template <class inputData>
void InputGenericVector<inputData>::save () {
    itemValues.push_back(make_pair(itemID,temp));
    temp.clear();
}
template <>
void InputGenericVector<double>::printVector(){
    for (unsigned int i=0; i<itemValues.size(); i++){
        //cout<<itemValues[i].second.size()<<endl;
        cout<<itemValues[i].first<<endl;
        for(unsigned int j=0; j<itemValues[i].second.size(); j++){
            cout<<itemValues[i].second.at(j)<<" ";
        }
        cout<<endl;
    }
}

template <>
void InputGenericVector<double>::constructorFunction(InputGenericVector<double>& vector,string const& value){
    vector.push(stod(value));

}

template <class inputData>
InputGenericVector<inputData>::InputGenericVector(string const& path){ //For input file
    //The input file
    ifstream inputFile;
    try{
        inputFile.open(path);
        if (!inputFile.is_open())
            throw "Can't open file";
        string firstLine;
        getline(inputFile, firstLine);  //read first line
        while (!inputFile.eof()) {
            string sLine;
            //Read line by line
            getline(inputFile, sLine);
            if(sLine.length()==0)//Break if it's last line.
                break;


            bool itemIDflag= true;
            string word = "";
            for (auto c:sLine ) {
                //Push every value delimited by white space
                if (iswspace(c)) {
                    if(!itemIDflag) { //If it is not the item's id
                        //this->push(stoi(sLine.substr(0, pos)));
                        constructorFunction(*this, word);           //push the created string
                    }
                    else{
                        itemID=word;
                        itemIDflag= false;
                    }
                    word = "";
                }
                else {
                    word = word + c;
                }

            }
            //for the last word of line
            if(!itemIDflag) { //If it is not the item's id
                //this->push(stoi(sLine.substr(0, pos)));
                constructorFunction(*this, word);
            }
            else{
                itemID=word;
                itemIDflag= false;
            }
            //Save the pushed values
            this->save();
        }

        inputFile.close();
    }catch (const char* msg) {
        cerr << msg << endl;
    }
}

template class InputGenericVector<double>;