## Cases Intended to be Covered in the Data
- varying correct values for *K* such that the answer cannot be checked for only small and large values of *K*
- graph where incrementing the value of *K* adds exponential number of paths consistently such that a solution can not be formed with a single Dijkstra allowing revisiting
- graph with max value of *K*
- graph with min value of *K*
- graph with no answer ("Love isn’t always on time!")
- graph where the sum of times across all paths from *0* to *n + 1* overflows integers
- dense graphs
- case where lots of nodes are disconnected from the network of substations connection the source to the destination
- line graphs which off-by-one solutions by making the weight of the path (*L*) *T - 1* <= *L* <= *T*
- graph where cut verticies on path have large divs
- SPFA break case with random orderings

Note: Evenly distribute value of *T* across all test cases
