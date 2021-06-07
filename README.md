# Graph Pattern Matching Challenge

## Member

2019-12731 강지혁  
2016-19620 정대용

## Report and Results

Report: ./Graph Pattern Matching Challenge Report.pdf  
Results: ./result

## Readme

각 출력 결과 마지막에 공백 (" ")이 하나 출력됩니다. 이점 양해 부탁드립니다.  

./result에 각 그래프당 최대 10만개의 매칭 결과를 출력한 .out 파일들이 있습니다.  
이 파일들의 경우, 시간을 측정해보기 위해 마지막 줄에 소요된 시간을 기입하였습니다.  
실제 출력 결과에는 시간이 출력되지 않음을 말씀드립니다.  

그 외, 형식과 관련한 문제나 에러에 대해서는 iwin1203@naver.com으로 이메일 주시면 감사하겠습니다.

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
