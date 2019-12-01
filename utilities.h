#ifndef SECOND_STEP_UTILITIES_H
#define SECOND_STEP_UTILITIES_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <chrono>
#include <tuple>

using namespace std;

void
assign_values_from_arguments(int const &argc, char **argv, string &inputFile, string &configurationFile, bool &complete,
                             int &whichInitialization, int &whichAssignment,
                             int &whichUpdate, string &outputFile);

void
assign_values_from_configurationFile(string const &file, unsigned int &k, unsigned int &L_grids, unsigned int &k_of_lsh,
                                     unsigned int &L_hashtables);

#endif //SECOND_STEP_UTILITIES_H
