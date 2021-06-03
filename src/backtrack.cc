/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <utility>
#include <stack>

using namespace std;

stack<pair<Vertex, Vertex>> Backtrack::stack;
vector<Vertex> Backtrack::mapping;
vector<Vertex> Backtrack::extendable;

vector<bool> Backtrack::visited;

// 05.31 대용 추가
set<Vertex> Backtrack::visitedQuery;
set<Vertex> Backtrack::notVisitedQuery;
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
void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {

  cout << "Hello world" << "\n";

  // data init
      mapping.reserve(query.GetNumVertices());
  // data init done

  // stack init
      Vertex root = 0;
      for(Vertex cds: cs.GetCandidateSet(root)){
          // Root is exception of DFS traverse in this case
          stack.push(pair<Vertex, Vertex>(root, cds));
      }
      mapping[root] = stack.top().second;
  // stack init done

  // Main start
      while(!stack.empty()){
          Vertex curr = stack.top().first;
          Vertex curr_m = stack.top().second;
          if(Map(curr, curr_m)) {
              mapping[curr] = curr_m;
              if (mapping.size() == query.GetNumVertices()) {
                  cout << "a ";
                  for_each(mapping.begin(), mapping.end(), [](Vertex v) { cout << v << " "; });
                  cout << "\n";
              } else {
                  pair<Vertex, vector<Vertex>> next = GetExtendable(data, query, cs); // error 처리가 안됨 -> (-1, -1) 받는 걸로 두자
                  for(Vertex v: next.second){
                      stack.push(pair<Vertex, Vertex>(next.first, v)); // what does it do when error?
                  }
              }
          }
          else{
              stack.pop();
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

    if(u < 0) return false;
    else{
        Backtrack::mapping[u] = v;
        return true;
    }
}


/**
 * [ Get Extendable ] extendable 한 Query Graph의 Vertex를 찾는다.
 *
 * @param query
 * @return
 */
pair<Vertex, vector<Vertex>> Backtrack::GetExtendable(const Graph &data, const Graph &query,
                                                      const CandidateSet &cs) {

    set<Vertex> extendables; // Extendable vertices in Query graph

    // visitedQuery.size() < notVisitedQuery.size() 라면, visited Query의 자식 중 not visited를 찾아 순회하는게 빠르다.
    if (visitedQuery.size() < notVisitedQuery.size()) {
        for (Vertex checked : visitedQuery) {
            for (Vertex child : childArray[checked]) {
                const bool valid = notVisitedQuery.find(child) != notVisitedQuery.end(); // nonvisited에 child가 있다.
                if (valid && CheckParent(child)) extendables.insert(child);
            }
        }
    }
        // visitedQuery.size() > notVisitedQuery.size() 라면, 그냥 not visitied를 곧장 순회하는게 빠르다.
    else {
        for (Vertex v : notVisitedQuery) {
            if (CheckParent(v)) extendables.insert(v);  // Extendable 한 Query Graph의 Vertices가 담겨있다.
        }
    }
    std::vector<Vertex> extArray(extendables.size());
    std::copy(extendables.begin(), extendables.end(), extArray.begin());
    pair<Vertex, vector<Vertex>> res = CountCMU(data, cs, extArray);

    return res;

}

pair<Vertex, vector<Vertex>> Backtrack:: CountCMU(const Graph &data, const CandidateSet &cs, vector<Vertex> extendables) {

    vector<int> cmuSizeArray;   // extendables의 원소들의 cmu size를 담을 배열
    vector<vector<Vertex>> availableVertex; // 각 extendable 별 가능한 vertex
    for (int i=0; i<extendables.size(); i++) {
        int eachExtendableCmuSize=0;
        vector<Vertex> eachExtendableVertex;
        for (Vertex v : cs.GetCandidateSet(extendables[i])) {   // extendable 별 cs 순회

            bool isContinue=false;
            for (Vertex toCheckVisited : visited) {
                if (toCheckVisited==v) isContinue=true; // 기존에 방문한 적 있으면 count않고 스킵
            }
            if (isContinue) continue;

            if (CheckCMU(data, extendables[i], v)) {    // CheckCMU(모든 부모와 연결되어있는지) 가 true면 cmu size 증가
                eachExtendableCmuSize++;
                eachExtendableVertex.push_back(v);      // 해당되는 vertex를 배열에 삽입
            }
        }
        cmuSizeArray.push_back(eachExtendableCmuSize);
        availableVertex.push_back(eachExtendableVertex);
    }

    int minIdx = min_element(cmuSizeArray.begin(), cmuSizeArray.end()) - cmuSizeArray.begin();  // 최소 cmu size의 idx 찾기
    pair<Vertex, vector<Vertex>> temp;
    temp.first = extendables[minIdx];       // first : 최소 cmu size의 extendable (u)
    temp.second = availableVertex[minIdx];  // second : 해당 u의 가능한 v
    return temp;
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
    int matchingNum=0;
    for (Vertex parent: parentArray[u]) {   // u의 부모들을 순회하며
        if (data.IsNeighbor(mapping[parent], v)) matchingNum++; // 부모가 map된 vertex와 인자 v가 연결되어있는지 확인
    }
    if (matchingNum==parentArray[u].size()) return true;
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
    for (int i=0; i<size; i++) {    // Query Graph를 순회한다.
        Vertex v = i;               // Query Graph 내의 Vertices를 id 순서대로 방문한다.
        std::vector<Vertex> perVertexParArr;
        std::vector<Vertex> perVertexCldArr;
        for (size_t j = query.GetNeighborStartOffset(v); j < query.GetNeighborEndOffset(v); ++j) {
            Vertex neighbor = query.GetNeighbor(j);
            if (neighbor<v)  perVertexParArr.push_back(neighbor);   // 각 Vertex의 neighbor 중 id가 작은 것을 담는다.
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
        const bool contains = notVisitedQuery.find(parent) != notVisitedQuery.end();
        if (contains) return false;
        return true;
    }
}



/*
 * 1. data를 global로 관리할까
 * 2. par/child arr = 똑같은 쿼리그래프 사이즈만큼의. 한번에 관리할 수 있다. 추가적인 함수를 둬서 list의 head를 갖고오게 한다면
 * par를 구할 수 있고 // <무거울 듯>.
 * 3. 붙여서 실행해보겠다.
 *
 * 하나만 날리잖아요.
 * 한번 뽑힌 얘는 가지고 있으면 됨.
 *
 */
