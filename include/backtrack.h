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

    void PrintAllMatches(const Graph &data, const Graph &query,
                         const CandidateSet &cs);

    //Vertex BuildRoot(const Graph &data, const Graph &query, const CandidateSet &cs);
    bool tryMap(Vertex u, Vertex v, const Graph &data, const Graph &query);
    pair<Vertex, vector<Vertex>> GetExtendable(const Graph &data, const Graph &query,
                                                          const CandidateSet &cs);
    vector<Vertex> GetCm(Vertex u, const Graph &data, const Graph &query, const CandidateSet &cs);
    void trace();
};


#endif  // BACKTRACK_H_
