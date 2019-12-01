#include "utilities.h"

void
assign_values_from_arguments(int const &argc, char **argv, string &inputFile, string &configurationFile, bool &complete,
                             int &whichInitialization, int &whichAssignment,
                             int &whichUpdate, string &outputFile) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0)
            inputFile = string(argv[i + 1]);
        else if (strcmp(argv[i], "-c") == 0)
            configurationFile = string(argv[i + 1]);
        else if (strcmp(argv[i], "-complete") == 0)
            complete = true;
        else if (strcmp(argv[i], "-interactive") == 0) {
            whichInitialization = stoi(argv[i + 1]);
            whichAssignment = stoi(argv[i + 2]);
            whichUpdate = stoi(argv[i + 3]);
        } else if (strcmp(argv[i], "-o") == 0)
            outputFile = string(argv[i + 1]);
    }
}

void
assign_values_from_configurationFile(string const &file, unsigned int &k, unsigned int &L_grids, unsigned int &k_of_lsh,
                                     unsigned int &L_hashtables) {
    ifstream configFile;
    try {
        configFile.open(file);
        if (!configFile.is_open())
            cout << "Can't open configuration file" << endl;
        while (!configFile.eof()) {
            string sLine;
            //Read line by line
            getline(configFile, sLine);
            if (sLine.length() == 0)//Break if it's last line.
                break;
            if (sLine.find("number_of_clusters:") != string::npos) {
                sLine.erase(0, 19);
                k = stoi(sLine);
            } else if (sLine.find("number_of_grids:") != string::npos) {
                sLine.erase(0, 16);
                L_grids = stoi(sLine);
            } else if (sLine.find("number_of_vector_hashtables:") != string::npos) {
                sLine.erase(0, 28);
                L_hashtables = stoi(sLine);
            } else if (sLine.find("number_of_vector_hashfunctions:") != string::npos) {
                sLine.erase(0, 31);
                k_of_lsh = stoi(sLine);
            }
        }

    } catch (const char *msg) {
        cerr << msg << endl;
    }
}