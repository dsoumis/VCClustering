Compile:make

Run:./cluster -i inputFile -c cluster.conf -o outputFile -complete -indicative 1 1 2 

1 1 2 are an examples of the combinations to use.First digit is which initialization 1.Initialization Simplest 2.Kmeans ++ Initialization. Second digit is which assignment 1.Assignment Simplest 2.Reverse Assignment. Third digit is which update 1.Update a la Loyd 2.Update simplest.

You can omit flag -complete for less detailed results in the outputFile.

inputFile: DataVectors_5_500x100.csv or trajectories_dataset_small.csv or others user created the follow the above files' patterns.

You can tamper with cluster.conf numbers.

outputFile: user defined path to file to print the results


Clustering Vectors:

Initialization Simplest: 

Initialize k centers by randomly selecting points from the given dataset checking each time that there is no duplicate center.

Kmeans ++ Initialization: 

Initially centers are stored in a vector (int) centers with the data InputGenericVector. We select a random int number in the interval [0, inputSize-1] and thus take the position of our first center in the vector with the data-InputGenericVector. Then manhattan distances D (i) are calculated for each point of the input: find the nearest center, that is, the distance with each center from the existing ones, and keep the shortest. Then calculate the partial sums P (i) for each input outside the centers, and calculate a float x in space [0.0, P (n-t)].

We try to find this value in vector P with a binary search function. If the number exists in the table the position of this cell is returned, otherwise we keep the cells l, r the cells left and right of the value we are looking for and return
bigger. This is also the new center we add to vector centers.

Assignment Simplest: 

The nearest center points based on metric l1 are assigned to the clusters.

Reverse Assignment: 

(LSH application) Insert dataset points into L hash tables. The centers are the queries of the LSH and depending on the bucket they correspond to, they control which points are less than or equal to a radius to assign these points to clusters. This procedure is repeated by doubling the radius each time until at least 70% of the dataset has been assigned to the clusters.
 
Update Simplest: 

Using the average of points in each dimension a new point is generated which is a new center for the next iteration of the algorithm. This step is done for each cluster.
 
Update a la Loyd: 

(Description by example)
Let cluster-1 contain {x, y, z} with center x. I calculate which of the x, y, z minimizes the sum of the distances between the points of cluster-1, even y. Y becomes the new center of cluster-1.
 
The algorithm terminates when the centers do not change after an iteration.
 
Clustering Curves:

Initialization Simplest: 

Initialize k centers by randomly selecting curves from the given dataset checking each time that there is no duplicate center.

Kmeans ++ Initialization: 

The procedure is the same as explained above, but the distances are calculated by DTW. All functions use template inputData and each function is specified twice where necessary, one for points and the other for curves.

Assignment Simplest:

The nearest center-centered DTW curves are assigned to the clusters.

Reverse Assignment: 

(LSH for curves application) Place the dataset curves in L grids. The centers are the queries of the LSH, and depending on the bucket they correspond to, they control which curves are less than or equal to a radius to assign these curves to clusters. This procedure is repeated by doubling the radius each time until at least 70% of the dataset has been assigned to the clusters.
 
 
 
 
Update Simplest:

Initialization

First, determine the length lamda of target sequence C.

Heuristically:λ=mean{len(S1),...,len(Sn)}=1/n∑len(Si).

Then initializeC:

1: Pick random sequence S0 among those of length≥lamda.

2. Take a length-lamda random subsequence from S0.

DBA ALGORITHM:

Input:

sequences S1,...,Sn, initial sequence C.

Output:centroid sequence C of the Si.

int lamda←len(C)

repeat

    seqC′←C
    array A←[∅,...,∅] //array of lamda pointsets
    for i=1,...,n do
        set IPairs←index-pairs of best-traversal(C,Si)
        for p∈IPairs do //p= (u,v)∈(N^∗)^2,u≤lamda
            A[p1]←A[p1]∪{Si[p2]} //pair=(C[p1],Si[p2])∈(R^d)^2
    for j=1,...,lamda do
        C[j]←mean(A[j])
until C′==C or DTW(C,C′)≤0.5
returnC


Update a la Loyd: 

(Description by example)
Let cluster-1 contain {x, y, z} with center x. I calculate which of the x, y, z minimizes the sum of the distances between the points of cluster-1, even y. Y becomes the new center of cluster-1.
 
The algorithm terminates when the centers do not change after an iteration or 10 iterations are completed.
 
 
The algorithm is evaluated by the Silhouette function.
 
It is worth noting that in addition to other optimizations a hash table is maintained in the form of unordered_map which maintains the already calculated distances to avoid recalculation and to reduce execution time.
 
Comparisons:

(1,1,1) The mean Silhouette is constantly changing from 0.3 to 0.7. Therefore, initial randomness plays a key role.

(1,2,1) The mean Silhouette is 0.47. It makes sense since LSH is used so the accuracy is definitely less than bruteforce. Updating the center with a point from the cluster though, has little benefit to the result.

(1,2,2) The mean Silhouette varies from 0.17 to 0.33. The worst case.

(1,1,2) The mean Silhouette varies from 0.27 to 0.7 (few times). Also a bad case.

(2,1,1) The mean Silhouette is 0.71 constant.

(2,1,2) The mean Silhouette is 0.71 constant.

(2,2,1) The mean Silhouette is 0.71.

(2,2,2) The mean Silhouette is 0.71.
 
Conclusion: The first choice of centers plays a decisive role in the outcome of each combination. Kmeans ++ initialization results in the best results.