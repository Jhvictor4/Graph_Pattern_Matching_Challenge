/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}



void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  std::cout << "t " << query.GetNumVertices() << "\n";
  std::cout<<"This is for test~"<<std::endl;

  
}


/*
 * [ 시작하자마자 진행 ]
 * 0. buildParent
 * 1. 한 vertex v의 neighbor 중 v 보다 id가 작은 것이 parent이다.
 * 2. query graph내의 모든 vertex에 대해 parent를 담은 이중 배열을 리턴한다.
 */

std::vector<std::vector<Vertex>> buildParent(Graph &query) {

    std::vector<std::vector<Vertex>> parArray;
    int size = query.GetNumVertices();

    for (int i=0; i<size; i++) {
        Vertex v = i;
        std::vector<Vertex> perVertexArr;
        for (size_t j = query.GetNeighborStartOffset(v); j < query.GetNeighborEndOffset(v); ++j) {
            Vertex neighbor = query.GetNeighbor(j);
            if (neighbor<v) { perVertexArr.push_back(neighbor); } // neighbor이 parent
        }
        parArray.push_back(perVertexArr);
    }
}

/*
 * [ q_D의 후보를 찾는 과정 ]
 * 0. checkParent
 * 1. 특정 vertex q의 부모 p가 지금까지 거쳐 온 vertex의 집합 Q에 존재하는지 확인한다.
 * 2. 모든 부모가 Q에 있으면 true를, 아니면 false를 리턴한다.
 */

bool checkParent(std::vector<std::vector<Vertex>> parArray, Vertex v, std::vector<Vertex> Q, std::vector<Vertex> notQ) {
    std::vector<Vertex> eachVecPar = parArray.at(v);

    int res = 0;
    for (Vertex p : eachVecPar) {
        for (Vertex k : Q) {
            if (p==k) res++;
        }
    }
    if (res == eachVecPar.size()) { return true; }
    return false;
}

/*
 * [ q_D의 후보를 찾는 과정 중 최적화 ]
 * 0. cut
 * 1. 특정 vertex가 cs 후보군으로 뽑혔다고 하자. 이때 이 vertex의 모든 자식들은 확인할 필요가 없다.
 * 2. 한 vertex v의 neighbor 중 v보다 큰 것들이 자식이다.
 * 3. 따라서 자식을 recursive하게 찾아서 그를 담은 집합을 리턴한다.
 * 4. 이 집합의 원소들은 cs 후보군을 찾을 때 처음부터 배제하도록 한다. (나름의 최적화)
 */

void cut(std::set<Vertex> cutSet, Vertex v, const Graph &query) {
    for (size_t j = query.GetNeighborStartOffset(v); j < query.GetNeighborEndOffset(v); ++j) { // 1차원 arr // 
        Vertex neighbor = query.GetNeighbor(j);
        if (neighbor>v) {
            cutSet.insert(neighbor);
            cut(cutSet, neighbor, query); // 재귀적으로 자식 다 찾기
        } // neighbor이 parent
    }
}

/*
 * [ C_M 구하는 중 연산 ]
 *
 */

int checkNeighborWithParVertex(Vertex v, std::vector<std::vector<Vertex>> parArray) {

    std::vector<Vertex> csVertices; // 얘도 미리 주어지면 좋음.
    // 현재 Q의 CS의 vertices
    std::vector<Vertex> selectedVertices; //  Selected vertex 모음
    /*
     *  selectedVertices 얘는 백트래킹 중 넘겨줘야 함
     */

    std::vector<Vertex> selectedVerticesFromParents;
    for (Vertex par : parArray.at(v)) { // q_D의 Vertex들을 의미
        for (Vertex parV: selectedVertices) { // CS 내의 선택된 v8과 v9를 의미.
            //if (parV belongs to CS(par)) {
            selectedVerticesFromParents.push_back(parV);
            //}
        }
    }
    /*
     * Q, 혹은 CS() 정보만으로 선택된 v 바로 특정할 수 있나???
     */

    int C_M = 0;
    for (Vertex v : csVertices) {
        int cnt =0;
        for (Vertex parV : selectedVerticesFromParents) { // v8 v9
            //if (parV. is neighbor with v ) {
                cnt++;
            //}
        }

        // mapping에서 가져오기.

        if (cnt == selectedVerticesFromParents.size()) {
            C_M++;
        }
    }
    return C_M;
}


/*
 * [ C_M 구하기 ]
 * 0. calculateCsSize
 * 1.
 *
 */
int calculateCsSize(std::vector<Vertex> tempCS, std::vector<std::vector<Vertex>> parArray, const CandidateSet &cs) {

    std::vector<int> csNum;
    for (Vertex u: tempCS) {
        int possibleVertex = 0;
        for (int i=0; i< cs.GetCandidateSize(u); i++) {
            // if checkNeighborWithParVertex((cs.GetCandidate(u, i))) {// checkNeighbor 후 연결 되어있으면
        // possibleVertex++;}
        }
    };
    int min = csNum.at(0);
    int minIdx = 0;
    for (int i=1; i < tempCS.size(); i++) {
        if (csNum.at(i)<min ) {
            min = csNum.at(i);
            minIdx=i;
        }
    }
    return minIdx; // 실제 연결된 v를 보내도 되고.

}

void candidateSizeOrder(std::vector<Vertex> Q, std::vector<Vertex> D, const Graph &data, const Graph &query,
                 const CandidateSet &cs) {
    // Q는 지금까지 선택된 query graph의 vertex들
    // notQ는 선택되지 않은 query graph의 vertex들.
    // D는 지금까지 선택된 data graph의 vertex들.

    std::vector<Vertex> notQ;

    std::vector<Vertex> tempCS; // 후보군이 담길 Q의 vertex

    // notQ 중 cs를 뽑기 with 최적화
    std::set<Vertex> cutArr;

    std::set<Vertex>::iterator iter;
    for (Vertex v : notQ) {
        iter = cutArr.find(v);
        if (iter==cutArr.end()) {
            continue;
        }
//        if (checkParent()) {
//         tempCS.pushback(v);
//         cut(cutArr, v)
//         }
    }
    // temp cs에 u4 u7이 담김

    /*
     * 루트 정하고 정해진 오더링에 으해서 백트래킹 돌잖아.
     * temp cs를 어떻게 돌지.
     * 이게 지금 ppt 20페이 보면 쿼리그래프 방향 有. 들어올땐 방향 없음. 루트를 정하는게 가장 중요.
     * 이해하고 있는게 맞다면 그렇게 해야. id가 작은 것이 par. 인접행렬 중 idx로 정해놓고.
     * root. 그래프 모양이 cycliclic
     */



}