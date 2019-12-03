OBJS_clustering    = BruteForceImplementation.o HashFunctions.o HashTables.o InputImplementation.o main.o clustering_curves.o clustering_vectors.o Dtw.o GridImplementation.o kmeansPlusPlus.o utilities.o
SOURCE_clustering  = BruteForceImplementation.cpp HashFunctions.cpp HashTables.cpp InputImplementation.cpp main.cpp clustering_curves.cpp clustering_vectors.cpp Dtw.cpp GridImplementation.cpp kmeansPlusPlus.cpp utilities.cpp
HEADER_clustering  = BruteForceImplementation.h HashFunctions.h HashTables.h InputImplementation.h clustering_curves.h clustering_vectors.h ClusterStructure.h Dtw.h GridImplementation.h HashPair.h kmeansPlusPlus.h utilities.h
OUT_clustering     = cluster

CC      = g++
FLAGS   = -g -c -O3 -std=c++11
# -g option enables debugging mode
# -c flag generates object code for separate files

all: 	$(OBJS_clustering)
		$(CC) -g $(OBJS_clustering) -o $(OUT_clustering)
		
# create/compile the individual files >>separately<<
BruteForceImplementation.o: BruteForceImplementation.cpp
	$(CC) $(FLAGS) BruteForceImplementation.cpp

HashFunctions.o: HashFunctions.cpp
	$(CC) $(FLAGS) HashFunctions.cpp

HashTables.o: HashTables.cpp
	$(CC) $(FLAGS) HashTables.cpp

InputImplementation.o: InputImplementation.cpp
	$(CC) $(FLAGS) InputImplementation.cpp

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

clustering_curves.o: clustering_curves.cpp
	$(CC) $(FLAGS) clustering_curves.cpp
	
clustering_vectors.o: clustering_vectors.cpp
	$(CC) $(FLAGS) clustering_vectors.cpp

Dtw.o: Dtw.cpp
	$(CC) $(FLAGS) Dtw.cpp

GridImplementation.o: GridImplementation.cpp
	$(CC) $(FLAGS) GridImplementation.cpp

kmeansPlusPlus.o: kmeansPlusPlus.cpp
	$(CC) $(FLAGS) kmeansPlusPlus.cpp

utilities.o: utilities.cpp
	$(CC) $(FLAGS) utilities.cpp	
# clean house
clean:
	rm -f $(OBJS_clustering) $(OUT_clustering)