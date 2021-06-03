/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <utility>
#include <stack>
#include <map>

using namespace std;

stack<pair<Vertex, Vertex>> Backtrack::stack;
map<Vertex, Vertex> Backtrack::mapping;

set<Vertex>  Backtrack::EV;
vector<vector<Vertex>> Backtrack::parentArray;
vector<vector<Vertex>> Backtrack::childArray;
//

Backtrack::Backtrack() {

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

/*
 * 1. u 뽑고나서 다시 왔을때 새로운 v를 택할 수 있도록 해야함
 * 2. 새로운 v는 id가 visited의 v보다 커야 함.
 *
 */




void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {

    cout << "Hello world" << "\n";

    // data init
    mapping.clear();
    EV.clear();
    // data init done
    BuildParentChild(query);

    /*
     * 이게 parent child 관계가 주어져 있으면 어떤 mapping에서 u가 빠졌으면 parent들을 조사.
     * u가 추가되면 child를 확인
     */
    // stack init
    Vertex root = 0;
    for (Vertex cds: cs.GetCandidateSet(root)) {
        // Root is exception of DFS traverse in this case
        stack.push(pair<Vertex, Vertex>(root, cds));
    }
    mapping[root] = stack.top().second;
    EV.insert(root);
    // stack init done

    // Main start
    while (!stack.empty()) {
        Vertex curr = stack.top().first;
        Vertex curr_m = stack.top().second;
        if (Map(curr, curr_m)) {
            mapping[curr] = curr_m;
            if (mapping.size() == query.GetNumVertices()) {
                cout << "a ";
                for_each(mapping.begin(), mapping.end(), [](pair<Vertex, Vertex> v) { cout << v.second << " "; }); //todo 잘 되나
                cout << "\n";
            } else {
                EV.erase(EV.find(curr)); // todo 오류 날 수도 있음
                for_each(childArray[curr].begin(), childArray[curr].end(), [this](Vertex child) {
                    if(CheckParent(child)) EV.insert(child);
                });

                pair<Vertex, vector<Vertex>> next = GetExtendable(data, query, cs); // todo error 처리가 안됨 -> 빈 vector로 일단 받음
                cout << next.first << " maps to ";
                for_each(next.second.begin(), next.second.end(), [](Vertex v){
                    cout << v << " ";
                });
                cout << "\n";

                for (Vertex v: next.second) {
                    stack.push(pair<Vertex, Vertex>(next.first, v));
                }
            }
        } else {
            cout << curr << " fails\n" ;
            stack.pop();
            mapping.erase(mapping.find(curr));
            // todo EV update // EV에 현재 u 빼주고 u의 par 넣어야함.
            for(Vertex u : parentArray[curr]){
                EV.insert(u);
            }
        }
    }
    // Main done

}

/**
 * Check if the last is valid mapping
 *
 * @param u
 * @param v
 * @return
 */
bool Backtrack::Map(Vertex u, Vertex v) {

    if (mapping[u] == v) return false;
    else {
        mapping[u] = v;
        return true;
    }
}


/**
 * [ Get Extendable ] extendable 한 Query Graph의 Vertex를 찾는다.
 *
 * cmu size array에 EV 원소들의 cmu size를 담음
 * availableVertex에 맵핑 가능한 vertex들을 담음 (datagraph의 vertex)들
 * EV를 순회하면서 (바깥for문)
 * 각 EV 원소에 속하는 candidates들을 cs.GetCandidateSet을 통해 받아옴 (안쪽 for문)
 * Check CMU를 통해 각 candidates들이 available한지 확인함 = 얘네가 extendable candidates
 * 결과적으로 Vector(pair(<EV원소>, <Extendable candidates>))가 완성!
 * 최소 CMU는 cmu size array의 최솟값의 idx를 찾고 그 idx를 availableVertex에 넣음 됨
 *
 * loop 1. build pair list
 * loop 2. traver pair list to pick smallest CmU size pair
 *
 *
 * @param query
 * @return
 */
pair<Vertex, vector<Vertex>> Backtrack::GetExtendable(const Graph &data, const Graph &query,
                                                      const CandidateSet &cs) {
    vector<int> cmuSizeArray;   // 배열
    vector<pair<Vertex, vector<Vertex>>> availableVertex; // 각 extendable 별 가능한 vertex

    for (Vertex u : EV) {
        int eachExtendableCmuSize = 0;
        vector<Vertex> eachExtendableVertex;
        pair<Vertex, vector<Vertex>> temp;

        //todo: CmU에 속해도 mapping 이 되어있는 경우 vertex를 포함해줄 수 없다
        for (Vertex eachV : cs.GetCandidateSet(u)) {
            if (CheckCMU(data, u, eachV)) {
                eachExtendableCmuSize++;
                eachExtendableVertex.push_back(eachV);
            }
        }

        cmuSizeArray.push_back(eachExtendableCmuSize);
        temp.first = u;
        temp.second = eachExtendableVertex;
        availableVertex.push_back(temp);
    }

    auto minIdx = min_element(cmuSizeArray.begin(), cmuSizeArray.end()) - cmuSizeArray.begin();  // 최소 cmu size의 idx 찾기
    pair<Vertex, vector<Vertex>> res = availableVertex.at(minIdx); // todo 이거 가능?


    return res;
}


/**
 * [ Check CM(u) ] Vertex v가 candidate set에 들어갈 수 있는지 (보라색으로 칠해지는지) 확인한다.
 *
 * @param data
 * @param u : Query Graph Vertex
 * @param v : Data Graph Vertex
 * @return true: v가 cs에 포함된다. / false: v가 cs에 포함되지 않는다.
 */
bool Backtrack::CheckCMU(const Graph &data, Vertex u, Vertex v) {
    // check whether v is in Cm(u) of u

    // visited
    for (auto &it : mapping) { if (it.second == v) return false; } // If already in mapping, then

    int matchingNum = 0;
    for (Vertex parent: parentArray[u]) {   // u의 부모들을 순회하며
        if (data.IsNeighbor(mapping[parent], v)) matchingNum++; // 부모가 map된 vertex와 인자 v가 연결되어있는지 확인
    }
    if (matchingNum == (int) parentArray[u].size()) return true;
    return false;
}


/**
 * [BUILD PARENT] Query Graph의 Vertices에 대하여, 특정 Vertex v의 neighbor 중 v보다 id가 작은 것을 parent로 둔다.
 *
 * @param query : Query Graph
 * @return : Query graph내의 모든 vertices에 대해 parent를 담은 이중 배열을 리턴한다.
 */

void Backtrack::BuildParentChild(const Graph &query) {

    int size = query.GetNumVertices();
    for (int i = 0; i < size; i++) {    // Query Graph를 순회한다.
        Vertex v = i;               // Query Graph 내의 Vertices를 id 순서대로 방문한다.
        std::vector<Vertex> perVertexParArr;
        std::vector<Vertex> perVertexCldArr;
        for (size_t j = query.GetNeighborStartOffset(v); j < query.GetNeighborEndOffset(v); ++j) {
            Vertex neighbor = query.GetNeighbor(j);
            if (neighbor < v) perVertexParArr.push_back(neighbor);   // 각 Vertex의 neighbor 중 id가 작은 것을 담는다.
            else perVertexCldArr.push_back(neighbor);
        }
        parentArray.push_back(perVertexParArr);
        childArray.push_back(perVertexCldArr);
    }
}

/**
 * [ CHECK PARENT ] Query Graph의 특정 Vertex v의 Parent가 이미 체크되었는지 확인한다.
 *
 * @param v : 부모를 확인하고자 하는 Query Graph의 Vertex
 * @return True : 모든 부모가 체크되었음 / False : 체크되지 않은 부모가 있음
 */

bool Backtrack::CheckParent(Vertex v) {
    for (Vertex parent : parentArray[v]) {
        if (mapping.find(parent) == mapping.end()) return false;
    }
    return true;
}

/*
 * Extendable vertex (보라색) 관리하는 array 하나로 통합하자. 편의상 EV라 하자.
 * 목표는 Visited Query, Not Visited Query 삭제하여 부하를 최소화 하는 것.
 *
 * #1
 * 루트를 EV에 넣고 맵핑한다.
 * Mapping = { (u1, v1) }
 * EV = { u1 }
 *
 * #2
 * EV의 Children 중 Check Parent가 된 얘들만 EV에 넣는다.
 * Mapping = { (u1, v1) }
 * EV = { u2 u3 u4 }
 *
 * #3
 * EV의 Cm(u) size를 구하고, Cm(u) size가 가장 작은 u를 선택한 후 맵핑한다.
 * 동시에, 선택된 u의 Children 중 Check Parent가 된 얘들만 EV에 추가한다.
 * Mapping = { (u1, v1), (u2, v2) } => u2를 EV에서 빼고 그 children u5 u6를 EV에 넣는다.
 * EV = { u5 u6 u3 u4 }
 *
 * #4
 * [반복] EV의 Cm(u) size를 구하고, Cm(u) size가 가장 작은 u를 선택한 후 맵핑한다.
 * 동시에, 선택된 u의 Children 중 Check Parent가 된 얘들만 EV에 추가한다. Child가 없으면 추가하지 않음.
 * [중요!] 이 경우 u6가 선택된다. 그러나 u6의 child인 u7은 아직 mapping에 u3가 없기 때문에 EV에 추가될 수 없다.
 * Mapping = { (u1, v1), (u2, v2) , (u6, v8) }
 * EV = { u5 u3 u4 }
 *
 * #5
 * [반복] EV의 Cm(u) size를 구하고, Cm(u) size가 가장 작은 u를 선택한 후 맵핑한다.
 * 동시에, 선택된 u의 Children 중 Check Parent가 된 얘들만 EV에 추가한다. Child가 없으면 추가하지 않음.
 * [중요!] (u5, v1)은 애초에 리턴되지 않음. getExtendable내에서 Mapping을 순회하며 이미 맵핑된 v1은 거르기 때문
 * Mapping = { (u1, v1), (u2, v2), (u6, v8), (u5, v7) }
 * EV = { u3 u4 }
 *
 * #6
 * Mapping = { (u1, v1), (u2, v2) , (u6, v8), (u5, v7), (u3, v4) }
 * EV = { u4 u7 }
 *
 * #7
 * Mapping = { (u1, v1), (u2, v2) , (u6, v8), (u5, v7), (u3, v4), (u7, v10) }
 * EV = { u4 u9 }
 *
 * #8
 * [중요] get extendable에서는 (u9,(v16, v17))을 리턴한다. 먼저 나온 v16으로 간다.
 * Mapping = { (u1, v1), (u2, v2) , (u6, v8), (u5, v7), (u3, v4), (u7, v10), (u9, v16) }
 * EV = { u4 }
 *
 * #9
 * [중요] u4를 get extendable 돌렸더니 결과가 null이다. (이미 v2가 맵핑되었으니까) => 망함
 * Mapping = { (u1, v1), (u2, v2) , (u6, v8), (u5, v7), (u3, v4), (u7, v10), (u9, v16) }
 * EV = { u4 }
 *
 * #10
 * => mapping의 마지막 pair인 (u9, v16)을 pop 하고 다시 (u9,(v16, v17))을 계산. v16보다 id가 큰 v17이 있으니 걔를 고름.
 * 맵핑이 되었으니 EV도 다시 계산 - 어차피 u9는 child없어서 변화 x
 * Mapping = { (u1, v1), (u2, v2) , (u6, v8), (u5, v7), (u3, v4), (u7, v10), (u9, v17) }
 * EV = { u4 }
 *
 * #11
 * [중요] u4를 get extendable 돌렸더니 결과가 null이다. (이미 v2가 맵핑되었으니까) => 망함
 * Mapping = { (u1, v1), (u2, v2) , (u6, v8), (u5, v7), (u3, v4), (u7, v10), (u9, v17) }
 * EV = { u4 }
 *
 * #12
 * => mapping의 마지막 pair인 (u9, v17)을 pop 하고 다시 (u9,(v16, v17))을 계산. v17보다 id가 큰 얘가 없으니 망함.
 * Mapping = { (u1, v1), (u2, v2) , (u6, v8), (u5, v7), (u3, v4), (u7, v10) }
 * EV = { u4 }
 *
 * ## 연속으로 backtrack 시작
 *
 * #13
 * => mapping의 마지막 pair인 (u7, v10) pop하고 다시 (u7, v10)을 계산. v10보다 id가 큰 얘가 없으니 망함.
 * Mapping = { (u1, v1), (u2, v2) , (u6, v8), (u5, v7), (u3, v4) }
 * EV = { u4 }
 *
 * #14
 * => mapping의 마지막 pair인 (u3, v4) pop하고 다시 (u3, (v4, v5, v6))을 계산. v4보다 id가 큰 v5 선택.
 * 맵핑이 되었으니 EV도 다시 계산
 * Mapping = { (u1, v1), (u2, v2), (u6, v8), (u5, v7), (u3, v5) }
 * EV = { u4 u7 }
 *
 * ** 성공한다면??
 *
 * #15
 * 완성된 mapping에서 마지막 backtracking 타며 선택된 v보다 id가 큰 v가 리턴되는지 확인. ex. (u8, (v13, v14, v15)) 중 14와 15 남았으니 u8로 backtrack
 * 최종 더 이상 선택된 v보다 id가 큰 v가 리턴되지 않으면 종료.
 */


