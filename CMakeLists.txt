cmake_minimum_required(VERSION 3.13)
project(second_step)

set(CMAKE_CXX_STANDARD 11)

add_executable(second_step main.cpp InputImplementation.cpp InputImplementation.h clustering_vectors.cpp clustering_vectors.h HashTables.h HashTables.cpp HashFunctions.h HashFunctions.cpp BruteForceImplementation.h BruteForceImplementation.cpp ClusterStructure.h clustering_curves.cpp clustering_curves.h Dtw.cpp Dtw.h GridImplementation.h GridImplementation.cpp HashPair.h kmeansPlusPlus.cpp kmeansPlusPlus.h utilities.cpp utilities.h)