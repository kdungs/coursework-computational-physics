#include "union_find.h"

UnionFind* UnionFind::Find() {
  if (this == parent_) {
    return this;
  }
  UnionFind *root = parent_->Find();
  parent_ = root;
  return root;
}

void UnionFind::Union(UnionFind *rhs) {
  UnionFind *lroot = Find(),
            *rroot = rhs->Find();
  if (lroot == rroot) {
    return;  // nothing to be done here
  }
  if (lroot->rank_ < rroot->rank_) {
    lroot->parent_ = rroot;
  } else if (lroot->rank_ > rroot->rank_) {
    rroot->parent_ = lroot;
  } else {
    rroot->parent_ = lroot;
    lroot->rank_ = rroot->rank_ + 1;
  }
}
