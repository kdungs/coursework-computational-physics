#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>

#include "union_find.h"


std::vector<bool> generate_grid(const size_t, const double, std::mt19937&);
std::vector<int> find_clusters(const std::vector<bool>&);
void print(const std::vector<int>&);


int main(int argc, char *argv[]) {
  // set up RNG
  std::random_device rd;
  std::mt19937 rng(rd());

  const size_t L = 50;
  auto grid = generate_grid(L, .5, rng);
  auto labels = find_clusters(grid);
  print(labels);
}


std::vector<bool> generate_grid(
  const size_t L,
  const double p,
  std::mt19937 &rng
) {
  static std::uniform_real_distribution<double> dist(0, 1);
  std::vector<bool> grid(L * L);
  std::generate(
    std::begin(grid),
    std::end(grid),
    [&] () { return dist(rng) < p; });
  return grid;
}

std::vector<int> find_clusters(const std::vector<bool> &grid) {
  std::vector<int> labels(grid.size());
  UnionFind<int> uf;
  const size_t L = sqrt(grid.size());

  // 1st step: Assign labels and figure out label equivalences
  int label_id = 1;
  size_t here,
         north,
         west;
  for (size_t y = 0; y < L; y++) {
    for (size_t x = 0; x < L; x++) {
      here = y * L + x;
      if (grid[here]) {
        north = here - L;
        west = here - 1;
        if (y > 0 && grid[north]) {
          if (x > 0 && grid[west] && labels[north] != labels[west]) {
            uf.Union(labels[north], labels[west]);
          }
          labels[here] = labels[north];
        } else if (x > 0 && grid[west]) {
          labels[here] = labels[west];
        } else {
          labels[here] = label_id;
          uf.MakeSet(label_id);
          ++label_id;
        }
      }
    }
  }
  // 2nd step
  std::transform(std::begin(labels), std::end(labels), std::begin(labels),
    [&] (const int &l) { return uf.Find(l); }
  );

  return labels;
}

void print(const std::vector<int> &labels) {
  const size_t L = sqrt(labels.size());
  for (size_t y = 0; y < L; y++) {
    for (size_t x = 0; x < L; x++) {
      const int v = labels[y * L + x];
      //if (v > 0) {
      //  std::cout << std::setw(2) << v;
      //} else {
      //  std::cout << "██";
      //}
      std::cout << v << " ";
    }
    std::cout << std::endl;
  }
}
