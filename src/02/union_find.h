#ifndef __UNION_FIND_H__
#define __UNION_FIND_H__

class UnionFind {
 private:
  UnionFind *parent_;
  unsigned int rank_;
  int id_;
 public:
  UnionFind() : parent_(this), rank_(0), id_(0) {};
  UnionFind* Find();
  void Union(UnionFind*);
  const int id() const { return id_; }
  void set_id(const int id) { id_ = id; }
};

#endif /* __UNION_FIND_H__ */
