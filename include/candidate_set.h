/**
 * @file candidate_set.h
 *
 */

#ifndef CANDIDATE_SET_H_
#define CANDIDATE_SET_H_

#include "common.h"

class CandidateSet {
 public:
  explicit CandidateSet(const std::string& filename);
  ~CandidateSet();

  inline size_t GetCandidateSize(Vertex u) const;
  inline Vertex GetCandidate(Vertex u, size_t i) const;
  //inline Vertex GetSmallest() const;
  inline std::vector<Vertex> GetCandidateSet(Vertex u) const;

 private:
  std::vector<std::vector<Vertex>> cs_;
};

/**
 * @brief Returns the number of data vertices that may be mapped to query vertex
 * u.
 *
 * @param u query vertex id.
 * @return size_t
 */
inline size_t CandidateSet::GetCandidateSize(Vertex u) const {
  return cs_[u].size();
}
/**
 * @brief Returns the i-th candidate from query vertex u's candidate set.
 *
 * @param u query vertex id.
 * @param i index in half-open interval [0, GetCandidateSetSize(u)).
 * @return Vertex
 */
inline Vertex CandidateSet::GetCandidate(Vertex u, size_t i) const {
  return cs_[u][i];
}

inline std::vector<Vertex> CandidateSet::GetCandidateSet(Vertex u) const {
    return cs_[u];
}

///**
// * return smallest value in the set
// */
//inline Vertex CandidateSet::GetSmallest() const {
//
//    size_t minimum = INT32_MAX;
//    Vertex result = 0;
//
//    for(Vertex u = 0; u < cs_.size(); u++){
//        if(cs_[u].size() < minimum){
//            minimum = cs_[u].size();
//            result = u;
//        }
//    }
//    return result;
//}
#endif  // CANDIDATE_SET_H_
