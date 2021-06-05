/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <utility>
#include <stack>
#include <map>
#include <queue>
#include <ctime>

using namespace std;

stack<pair<Vertex, Vertex>> Backtrack::stack;
map<Vertex, Vertex> Backtrack::mapping;
set<Vertex> Backtrack::EV;

Backtrack::Backtrack() {}

Backtrack::~Backtrack() = default;

/**
 * Backtracking - Main method of this hw
 * @param data
 * @param query
 * @param cs
 *
 * Use Dynamic Programming
 *
 * Workflow
 * 1: map current (u,v) pair to Embedding
 * 2: find extendable vertex u
 *    -> Candidate-size order; Choose u such that has min(Cm(u)).
 * 3: recurse
 *
 * replace printf to output file before submit!
 */
void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {

    mapping.clear(); EV.clear();

    /// Root : 아무거나 잡아도 되는 게 맞다.
    Vertex root = 0;
    for(Vertex v : cs.GetCandidateSet(root)){
        stack.push(pair<Vertex, Vertex>(root, v));
    }

    /// information field
    size_t N = query.GetNumVertices();       // N = query size
    bool credit = true;                      // flag to tell each step's result (success / fail)
    //int cnt = 0;
    //clock_t start = clock();
    /// declaration done

    cout << "t " << N << "\n";
    while(!stack.empty())
    {
        //if(clock() - start >= 60000) break;

        Vertex curr = stack.top().first, curr_m = stack.top().second;
        /// 1. try Mapping - test passed
        if( credit )
        {
            mapping[curr] = curr_m;
            /// 2. when success
            if( mapping.size() == N )
            {
                /// 2-1. if all are mapped, print one
                //cnt++;
                cout << "a ";
                for_each(mapping.begin(), mapping.end(), [](pair<Vertex, Vertex> pair){
                    cout << pair.second << " ";
                });
                cout << "\b\n";
                credit = false;
            }
            else
            {
                /// 2-2. if there's more to map, go ahead

                    /// update EV
                    auto it = EV.find(curr);
                    if( it != EV.end()) EV.erase(it);
                    for(size_t idx = query.GetNeighborStartOffset(curr); idx < query.GetNeighborEndOffset(curr); idx++ ){
                        Vertex neighbor_of_u = query.GetNeighbor(idx);
                        if(mapping.find(neighbor_of_u) == mapping.end()) EV.insert(neighbor_of_u); // in this condition, neighbor_of_u = child[u]
                        // todo: 부모 중에 mapping 에 없는 놈이 있으면 reject 했으나, 새로운 방식에서 해당 경우에는 부모의 정의에 위배되므로 부모라 할 수 없고, 그래서 그냥 일단 넘어간다.
                    }
                    /// update done

                    /// 2-2-1. Get next extendable Vertex
                    pair<Vertex, vector<Vertex>> next = GetExtendable(data, query, cs); // if empty vector comes in, then it means failure, then restart

                    /// 2-2-2. Push next to Stack
                    if(next.second.empty()){
                        credit = false;
                        /// fail if nothing is pushed, erase added EV
                        for(size_t idx = query.GetNeighborStartOffset(curr); idx < query.GetNeighborEndOffset(curr); idx++ ){
                            Vertex neighbor_of_u = query.GetNeighbor(idx);
                            if(mapping.find(neighbor_of_u) == mapping.end() && (EV.find(neighbor_of_u) != EV.end())) EV.erase(EV.find(neighbor_of_u));
                        }
                        EV.insert(curr);
                        /// rollback done
                    }
                    for (Vertex v: next.second) {
                        pair<Vertex, Vertex> t = pair<Vertex, Vertex>(next.first, v);
                        stack.push(t);
                    }
            }
        }
        else
        {
            /// 3. if fails

                /// 3-1. if mapping maintains, erase it
                mapping.erase(mapping.find(curr));
                /// 3-2. pop (curr, curr_m) pair
                stack.pop();
                /// 3-3. check if stack.top is the same as curr -> pop until only v is changed
                while(!stack.empty() && stack.top().first != curr){
                    curr = stack.top().first;
                    curr_m = stack.top().second;
                    /// fail if nothing is pushed, erase added EV
                    for(size_t idx = query.GetNeighborStartOffset(curr); idx < query.GetNeighborEndOffset(curr); idx++ ){
                        Vertex neighbor_of_u = query.GetNeighbor(idx);
                        if(mapping.find(neighbor_of_u) == mapping.end() && (EV.find(neighbor_of_u) != EV.end())) EV.erase(EV.find(neighbor_of_u));
                    }
                    EV.insert(curr);
                    /// rollback done
                    mapping.erase(mapping.find(curr));
                    stack.pop();
                }
                credit = true;
                /// EV update scheme ->
        }
        //trace();
    }
    //cout << "elapsed time: " << clock()-start << "ms, total cnt: " << cnt << endl;
    //cout << cnt << endl;
}


/**
 * tryMap : check if mapping (u, v) is available
 *
 * @param u
 * @param v
 * @param data
 * @param query
 * @return
 */
bool Backtrack::tryMap(Vertex u, Vertex v, const Graph &data, const Graph &query){


    /// approaching twice -> fails always
    auto it = mapping.find(u);
    if( (it != mapping.end()) && it->second == v) return false;

    /// for all u's neighbor (possible parent_u); IF neighbor is already in mapping then it is u's parent
    for(size_t offset = query.GetNeighborStartOffset(u); offset < query.GetNeighborEndOffset(u); offset++){
        Vertex parent_u = query.GetNeighbor(offset);
        /// IF parent_u is in mapping AND parent_v is not a neighbor with v -> map fails
        if( (mapping.find(parent_u) != mapping.end()) && !data.IsNeighbor(v, mapping[parent_u])) return false;
    }

    /// check if vertex v is already mapped on embedding
    for(auto pair: mapping){
        if(pair.second==v) return false;
    }
    return true;
}

/**
 * GetExtendable: fetches next vertex "u" and its extendable candidates "Cm(u)" according to Candidate Size ordering
 *
 * @param data
 * @param query
 * @param cs
 * @return
 */
pair<Vertex, vector<Vertex>> Backtrack::GetExtendable(const Graph &data, const Graph &query,
                                                      const CandidateSet &cs){

    pair<Vertex, vector<Vertex>> next = {};
    size_t size = INT32_MAX;

    /// candidate size ordering;
    for(Vertex extendable_u : EV){
        vector<Vertex> tmpSet = GetCm(extendable_u, data, query, cs);
        size_t tmp = tmpSet.size();
        if(tmp < size){
            size = tmp;
            next = pair<Vertex, vector<Vertex>>(extendable_u, tmpSet);
        }
    }
    return next;
}

/**
 * GetCm: fetches Cm(u) for given vertex "u"
 *
 * @param u
 * @param data
 * @param query
 * @param cs
 * @return
 */
vector<Vertex> Backtrack::GetCm(Vertex u, const Graph &data, const Graph &query, const CandidateSet &cs){

    vector<Vertex> res = {};
    /// for all u's neighbor, if something is mapped(parent_u), and mapped one(parent_v) links to some v in C(u)
    for(Vertex candidate : cs.GetCandidateSet(u)){
        if(tryMap(u, candidate, data, query)) res.push_back(candidate);
    }
    return res;
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

