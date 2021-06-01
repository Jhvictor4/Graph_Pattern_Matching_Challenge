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

using namespace std;

class Backtrack {
public:
    Backtrack();

    ~Backtrack();

    static vector<Vertex> mapping;
    static vector<bool> visited;

    // 05.31 대용 추가
    static set<Vertex> visitedQuery;
    static set<Vertex> notVisitedQuery;
    static vector<vector<Vertex>> parentArray;
    static vector<vector<Vertex>> childArray;
    //

    void PrintAllMatches(const Graph &data, const Graph &query,
                         const CandidateSet &cs);

    bool Map(const Graph &data, Vertex u, const vector<Vertex> &candidates);

    bool CheckCMU(const Graph &data, Vertex u, Vertex v);


    void BuildParentChild(const Graph &query);
    bool CheckParent(Vertex v);
    pair<Vertex, vector<Vertex>>GetExtendable(const Graph &data, const Graph &query, const CandidateSet &cs);
    pair<Vertex, vector<Vertex>> CountCMU(const Graph &data, const CandidateSet &cs, vector<Vertex> extendables);
};

#endif  // BACKTRACK_H_
