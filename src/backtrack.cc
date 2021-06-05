/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <utility>
#include <stack>
#include <map>
#include <queue>

using namespace std;

stack<pair<Vertex, Vertex>> Backtrack::stack;
map<Vertex, Vertex> Backtrack::mapping;
set<Vertex>  Backtrack::EV;

vector<vector<Vertex>> Backtrack::parentArray;
vector<vector<Vertex>> Backtrack::childArray;


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


void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {

    // data init
    mapping.clear();
    EV.clear();
    // data init done
    BuildParentChild(query);

    // stack init
    Vertex root = 35;
    for (Vertex cds: cs.GetCandidateSet(root)) {
        // Root is exception of DFS traverse in this case
        stack.push(pair<Vertex, Vertex>(root, cds));
    }
    EV.insert(root);
    trace();
    // stack init done

    // Main start
    while (!stack.empty()) {
        Vertex curr = stack.top().first;
        Vertex curr_m = stack.top().second;
        cout << curr << " " << curr_m <<"\n";
        if (Map(curr, curr_m)) {
            mapping[curr] = curr_m;
            if (mapping.size() == query.GetNumVertices()) {
                cout << "*************************************************************************************** and last vertex was (" << curr << ", " << curr_m << ")\n";
                cout << "a ";
                vector<Vertex> match = {};
                for_each(mapping.begin(), mapping.end(), [match](pair<Vertex, Vertex> v) mutable{
                    cout << v.second << " ";
                    match.push_back(v.second);
                });
                cout << "\n";
                //checker(match, data, query, cs);
            } else {

                auto it = EV.find(curr);
                if( it != EV.end()) EV.erase(it); // todo 오류 날 수도 있음

                for_each(childArray[curr].begin(), childArray[curr].end(), [this](Vertex child) {
                    if(CheckParent(child)) EV.insert(child);
                });

                pair<Vertex, vector<Vertex>> next = GetExtendable(data, query, cs); // todo error 처리가 안됨 -> 빈 vector로 일단 받음

                for (Vertex v: next.second) {
                    pair<Vertex, Vertex> t = pair<Vertex, Vertex>(next.first, v);
                    stack.push(t);
                }
            }
        } else {

            //cout << "************\n";
            mapping.erase(mapping.find(curr));
            stack.pop();
            while(!stack.empty() && stack.top().first != curr){
                //cout << curr << " " << curr_m << "\n";
                curr = stack.top().first;
                curr_m = stack.top().second;
                mapping.erase(mapping.find(curr));
                stack.pop();
            }
            EV.clear();
            //cout << "************\n";

        }
        trace();
    }
    // Main done

}


bool Backtrack::Map(Vertex u, Vertex v) {


    if ( mapping[u] == v) return false;
    else {
        //cout << "mapped " << u <<" "<< v << "\n";
        mapping[u] = v;
        return true;
    }
}


pair<Vertex, vector<Vertex>> Backtrack::GetExtendable(const Graph &data, const Graph &query,
                                                      const CandidateSet &cs) {
    vector<int> cmuSizeArray;   // 배열
    vector<pair<Vertex, vector<Vertex>>> availableVertex; // 각 extendable 별 가능한 vertex

    for (Vertex u : EV) {
        int eachExtendableCmuSize = 0;
        vector<Vertex> eachExtendableVertex;
        pair<Vertex, vector<Vertex>> temp;

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
    pair<Vertex, vector<Vertex>> res;
    if(availableVertex.empty()){
        vector<Vertex> tmp = {};
        res = pair<Vertex, vector<Vertex>>(-1, tmp);
    }
    else res = availableVertex.at(minIdx); // todo 이거 가능?

    return res;
}



bool Backtrack::CheckCMU(const Graph &data, Vertex u, Vertex v) {

    // check whether v is in Cm(u) of u
    // visited
    // for (auto &it : mapping) { if (it.second == v) return false; } // If already in mapping, then

    int matchingNum = 0;
    for (Vertex parent: parentArray[u]) {   // u의 부모들을 순회하며
        if (data.IsNeighbor(mapping[parent], v)) matchingNum++; // 부모가 map된 vertex와 인자 v가 연결되어있는지 확인
    }
    cout << "CheckCMU of vertex " << u << ", " << "v ->" << matchingNum << " " << parentArray[u].size() ;
    if (matchingNum == (int) parentArray[u].size()) return true;
    return false;
}


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


bool Backtrack::CheckParent(Vertex v) {
    for (Vertex parent : parentArray[v]) {
        if (mapping.find(parent) == mapping.end()) return false;
    }
    return true;
}

void Backtrack::trace(){

    cout << "***************************************************************\n";
    cout << "stack\n";
    cout << "size: " << stack.size();
    if(!stack.empty()) cout << "\ntop:  (" << stack.top().first << ", " << stack.top().second << ")";
    cout << "\nmapping\n[ ";
    for_each( mapping.begin(), mapping.end(), [](pair<Vertex, Vertex> p){
        cout << "(" << p.first << ", " << p.second << ") ";
    });
    cout << "]\nEV \n[ ";
    for(Vertex u : EV){
        cout << u << " ";
    }
    cout << "]\n";
    cout << "***************************************************************\n";
}

/**
 * possibly merge with buildParentChild
 *
 * @param data
 * @param query
 * @param cs
 * @return
 */
Vertex Backtrack::makeRoot(const Graph &data, const Graph &query,
                           const CandidateSet &cs){

    double min = INT_MAX;
    Vertex root = 0;
    for(Vertex u = 0; (size_t) u < query.GetNumVertices(); u++ ){
        int cnt = 0;
        for(Vertex v: cs.GetCandidateSet(u)){
            if(query.GetDegree(u) <= data.GetDegree(v)) cnt++;
        }
        double arg = ((double) cnt) / query.GetDegree(u);
        if(arg < min){
            root = u;
            min = arg;
        }
    }
    return root;
}

