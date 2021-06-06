# Graph Pattern Matching Challenge

## Member

2019-12731 강지혁  2016-19620 정대용

## Report and Results

Report: ./Graph Pattern Matching Challenge Report.pdf
Results: ./result


## Compile and Execute 
### main program
```
mkdir build
cd build
cmake ..
make
./main/program <data graph file> <query graph file> <candidate set file>
```
### executable program that outputs a candidate set
```
./executable/filter_vertices <data graph file> <query graph file>
```
### References
[1] Myoungji Han, Hyunjoon Kim, Geonmo Gu, Kunsoo Park, and Wook-Shin Han. 2019. Efficient Subgraph Matching: Harmonizing Dynamic Programming, Adaptive Matching Order, and Failing Set Together. In Proceedings of the 2019 International Conference on Management of Data (SIGMOD '19). Association for Computing Machinery, New York, NY, USA, 1429–1446. DOI:https://doi.org/10.1145/3299869.3319880
