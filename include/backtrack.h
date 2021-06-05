/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"
#include <utility>
#include <stack>
#include <map>

using namespace std;

class Backtrack {
public:
    Backtrack();

    ~Backtrack();

    static stack<pair<Vertex, Vertex>> stack;
    static map<Vertex, Vertex> mapping;
    static set<Vertex> EV;

    static vector<vector<Vertex>> parentArray;
    static vector<vector<Vertex>> childArray;

    Vertex makeRoot(const Graph &data, const Graph &query,
                    const CandidateSet &cs);
    void PrintAllMatches(const Graph &data, const Graph &query,
                         const CandidateSet &cs);
    bool Map(Vertex u, Vertex v);
    bool CheckCMU(const Graph &data, Vertex u, Vertex v);
    void BuildParentChild(const Graph &query);
    bool CheckParent(Vertex v);
    pair<Vertex, vector<Vertex>>GetExtendable(const Graph &data, const Graph &query, const CandidateSet &cs);
    void trace();
//    Vertex getMatching(Vertex u, vector<bool> dataFlag, vector<Vertex> matched, const CandidateSet &cs);
//    void checker(const vector<Vertex> matched, const Graph &data, const Graph &query, const CandidateSet &cs);
};


#endif  // BACKTRACK_H_
