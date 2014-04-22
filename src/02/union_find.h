#ifndef __UNION_FIND_H__
#define __UNION_FIND_H__

#include <map>

template <typename T>
class UnionFind {
 private:
  std::map<T, T> parents_;
 public:
  void MakeSet(const T x) { parents_[x] = x; }
  void Union(const T x, const T y) {
    const T xroot = Find(x),
            yroot = Find(y);
    if (xroot < yroot) {
      parents_[yroot] = xroot;
    } else {
      parents_[xroot] = yroot;
    }
  }
  const T Find(const T x) {
    const T p = parents_[x];
    if (p == x) {
      return x;
    }
    return Find(p);
  }
};

#endif /* __UNION_FIND_H__ */
