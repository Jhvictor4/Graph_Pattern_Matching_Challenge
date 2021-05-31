/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

using namespace std;

Backtrack::Backtrack() {
    mapping = {};
    visited = {};
}
Backtrack::~Backtrack() = default;

/**
 * Backtracking - Main method of this hw
 * @param data
 * @param query
 * @param cs
 *
 * Use Dynamic Programming; public Pair<Vertex, Vertex>
 *
 * Workflow
 * 1: Get an arbitrary root in Query graph - do the same as step 2, 3 in practice (not sure)
 * 2: find extendable vertex u
 *    -> Candidate-size order; Choose u such that has min(Cm(u)).
 * 3: In Cm(u), select each one like DFS and recurse
 *
 * replace printf to output file before submit!
 */
void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {

  cout << "Hello world" << "\n";

    // step 0 -> initialize Mapping & Visited; mapping has -1, visited has false
    mapping.resize(query.GetNumVertices());
    for_each(mapping.begin(), mapping.end(), [](Vertex u){ u = -1; });
    visited.resize(data.GetNumVertices());
    cout << "t " << query.GetNumVertices() << "\n";

    // step 1 -> "find root"; find u such that has minimum # in CS - not sure yet, just start with id 0
    Vertex root = 0;

    // step 2 -> start backtracking; Cm(u) 안의 v들에 대해 dfs 순으로 탐색
    Vertex uid = root;
    mapping[uid] = cs.GetCandidate(uid, 0); // todo: does root(u with id 0) always have one candidate v?
    while( !mapping.empty() ){
        if(Map(uid, cs.GetCandidateSet(uid))){
            if(mapping.size() == query.GetNumVertices()){
                cout << "a ";
                for_each(mapping.begin(), mapping.end(), [](Vertex v){ cout << v << " "; });
                cout << "\n";
            }
            else {
                // move to next u (extendable)
                // todo: implement fetching extendable U (check parent,,, do something)
                uid = GetExtendable();
            }
        }
        else{
            // if Map is false -> it's not updated
            // back to previous u
            uid = mapping.back();
            mapping.pop_back();
        }
    }
}

/**
 * Mapping : find (u, v) pair
 * -> extendable u
 * -> unvisited v (v in Cm(u))
 *
 * return
 * true : successfully mapped ; added (u, v)
 * false : no mapping available ; mapping is not updated
 */
bool Backtrack::Map(Vertex u, const vector<Vertex>& candidates) {

    for(int v : candidates){
        if( !visited[v] && CheckCMU(u, v) ){
            // first - fit search
            mapping[u] = v;
            visited[v] = true;
            return true;
        }
    }
    return false;
}

// todo: Implement CheckCMU -> is this v (data vertex) has all Cm(u_p) as its parents? (u_p -> u's parents)

bool Backtrack::CheckCMU(Vertex u, Vertex v) {
    // check whether v is in Cm(u) of u
}

Vertex Backtrack::GetExtendable() {
    //get extendable U with some logic
}


/**
 *  upper : 지혁
 *  below : 대용
 */


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
    for (size_t j = query.GetNeighborStartOffset(v); j < query.GetNeighborEndOffset(v); ++j) {
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

void checkNeighborWithParVertice() {

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
            // if checkNeighborWithParVertice((cs.GetCandidate(u, i))) {// checkNeighbor 후 연결 되어있으면
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
    std::vector<Vertex> tempCS;

    // notQ 중 cs를 뽑기 with 최적화
    std::set<Vertex> cutArr;
    std::set<Vertex>::iterator iter;
    for (Vertex v : notQ) {
        iter = cutArr.find(v);
        if (iter==cutArr.end()) {
            continue;
        }
        //if (checkParent()) {
        // tempCS.pushback(v);
        // cut(cutArr, v)
        // }
    }

}