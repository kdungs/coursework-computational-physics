#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <utility>
#include <vector>
#include <set>

#include "union_find.h"


int main(int argc, char *argv[]) {
  const size_t L = 100;
  const double p = .75;

  // set up RNG
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  // generate grid
  std::vector<bool> grid(L * L);
  std::generate(std::begin(grid), std::end(grid),
    [&] () { return dist(rng) < p; }
  );

  // Two-Pass algorithm
  // 1st step: assign labels
  std::vector<UnionFind> label_store(L * L);
  std::vector<UnionFind*> labels(L * L);
  size_t y,
         x;
  int label_id = 1;
  UnionFind *empty = &label_store[0];
  for (y = 0; y < L; y++) {
    for (x = 0; x < L; x++) {
      const size_t here = y * L + x;
      if (!grid[here]) {
        labels[here] = empty;
      }
      else {
        const size_t north = here - L,
                     west = here - 1;
        bool nnorth = false,
             nwest = false;
        UnionFind *lnorth = empty,
                  *lwest = empty;
        if (y > 0) {
          nnorth = grid[north];
          lnorth = labels[north];
        }
        if (x > 0) {
          nwest = grid[west];
          lwest = labels[west];
        }
        if (nnorth) {
          if (nwest && lnorth != lwest) {
            lnorth->Union(lwest);
          }
          labels[here] = lnorth;
        } else if (nwest) {
          labels[here] = lwest;
        } else {
          labels[here] = &label_store[label_id];
          label_store[label_id].set_id(label_id);
          ++label_id;
        }
      }
    }
  }
  // 2nd step: reduce labels and make them printable
  std::vector<int> printable_labels(L * L);
  std::transform(
    std::begin(labels),
    std::end(labels),
    std::begin(printable_labels),
    [] (UnionFind *l) {
      return l->Find()->id();
    }
  );
  
  // print stuff
  for (y = 0; y < L; y++) {
    for (x = 0; x < L; x++) {
      auto v = printable_labels[y * L + x];
      std::cout << v << " ";
      //if (v != 0) {
      //  std::cout << std::setw(2) << v << " ";
      //} else {
      //  std::cout << "█ ";
      //}
    }
    std::cout << std::endl;
  }
}
