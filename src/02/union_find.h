/** @file union_find.h
 * Implement a union-find data structure used in cluster finding algorithm.
 */
#ifndef __UNION_FIND_H__
#define __UNION_FIND_H__

#include <map>

/** Data structure used in cluster finding algorithm.
 */
template <typename T>
class UnionFind {
 private:
  std::map<T, T> parents_;
 public:
  /** Add variable to data structure. Parent is set to the variable itself.
   * @param x the variable that is added to the structure
   */
  void MakeSet(const T x) { parents_[x] = x; }
  /** Construct equivalence of two variables.
   * @param x, y the variables that are equivalent
   */
  void Union(const T x, const T y) {
    const T xroot = Find(x),
            yroot = Find(y);
    if (xroot < yroot) {
      parents_[yroot] = xroot;
    } else {
      parents_[xroot] = yroot;
    }
  }
  /** Find representation of equivalence class of given variable.
   * @param x variable for which the representation is returned
   * @return representation of x
   */
  const T Find(const T x) {
    const T p = parents_[x];
    if (p == x) {
      return x;
    }
    return Find(p);
  }
};

#endif /* __UNION_FIND_H__ */
