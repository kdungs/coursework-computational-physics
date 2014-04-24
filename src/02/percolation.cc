/** @file perculation.cc
 * Implement several functions relevant for simulations concerning perculating
 * clusters.
 */
#include <algorithm>
#include <cmath>
#include <fstream>
#include <random>
#include <vector>

#include "union_find.h"
#include "perculation.h"


/** Generate a grid of size (L, L) where each point has been visited with
 * probability p.
 * The memory efficiency of this depends on return value optimisation in the
 * compiler.
 * @param L size of the grid
 * @param p probability for each point
 * @param rng reference to a RNG
 * @return vector of booleans indicating whether point has been visited a point
 * (x, y) resides at position y * L + x in the vector.
 */
std::vector<bool>
generateGrid(const size_t L, const double p, std::mt19937 &rng) {
  static std::uniform_real_distribution<double> dist(0, 1);
  std::vector<bool> grid(L * L);
  std::generate(
    std::begin(grid),
    std::end(grid),
    [&] () { return dist(rng) < p; });
  return grid;
}


/** Find all clusters on a given grid as created by generateGrid().
 * Based on a two-step algorithm using a UnionFind data structure.
 * Also uses return value optimisation.
 * @param grid occupied points
 * @return vector of labels indicating to which cluster a point belongs; 0
 * means that the point is not occupied
 */
std::vector<int> findClusters(const std::vector<bool> &grid) {
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


/** Finds label of perculating cluster in vector of labels.
 * @param labels the vector of the labels
 * @return label of the perculating cluster (there can only be one) or 0 if no
 * perculating cluster was found
 */
int findPerculating(const std::vector<int> &labels) {
  const size_t L = sqrt(labels.size());
  std::vector<int> top(L),  // top row
                   bottom(L),  // bottom row
                   left(L),  // leftmost column
                   right(L);  // rightmost column
  for (size_t i = 0; i < L; i++) {
    top[i] = labels[i];
    bottom[i] = labels[L * L - 1 - i];
    left[i] = labels[i * L];
    right[i] = labels[(i + 1) * L - 1];
  }
  std::sort(std::begin(top), std::end(top));
  std::sort(std::begin(bottom), std::end(bottom));
  std::sort(std::begin(left), std::end(left));
  std::sort(std::begin(right), std::end(right));

  // remove all 0
  auto is_zero = [] (const int &x) { return x == 0; };
  auto new_end = std::remove_if(std::begin(top), std::end(top), is_zero);
  top.resize(std::distance(std::begin(top), new_end));
  new_end = std::remove_if(std::begin(bottom), std::end(bottom), is_zero);
  bottom.resize(std::distance(std::begin(bottom), new_end));
  new_end = std::remove_if(std::begin(left), std::end(left), is_zero);
  left.resize(std::distance(std::begin(left), new_end));
  new_end = std::remove_if(std::begin(right), std::end(right), is_zero);
  right.resize(std::distance(std::begin(right), new_end));

  std::vector<int> topbottom(std::min(top.size(), bottom.size())),
                   leftright(std::min(left.size(), right.size()));
  auto it = std::set_intersection(
    std::begin(top), std::end(top),
    std::begin(bottom), std::end(bottom),
    std::begin(topbottom)
  );
  topbottom.resize(std::distance(std::begin(topbottom), it));
  it = std::set_intersection(
    std::begin(left), std::end(left),
    std::begin(right), std::end(right),
    std::begin(leftright)
  );
  leftright.resize(std::distance(std::begin(leftright), it));
  
  std::vector<int> result(std::min(topbottom.size(), leftright.size()));
  it = std::set_intersection(
    std::begin(topbottom), std::end(topbottom),
    std::begin(leftright), std::end(leftright),
    std::begin(result)
  );
  result.resize(std::distance(std::begin(result), it));
  if (result.size() < 1) {
    return 0;
  }
  return result[0];
}


/** Simulate the relative perculation rate q_L(p) for a given probability p on
 * R clusters of size L * L.
 * @param R number of realisations
 * @param L size of grid
 * @param p probability with wich a point on the grid is occupied
 * return q_L(p)
 */
double simulateRelativePerculationRate(
  const size_t R,
  const size_t L,
  const double p,
  std::mt19937 &rng
) {
  size_t n_perc = 0;
  for (size_t i = 0; i < R; i++) {
    auto grid = generateGrid(L, p, rng);
    auto labels = findClusters(grid);
    if (findPerculating(labels) > 0) {
      ++n_perc;
    }
  }
  return n_perc / R;
}


/** Calculate size for each cluster.
 * @param labels vector of cluster labels on a grid
 * @return map[cluster id] = cluster size
 */
std::map<int, size_t> clusterSizes(const std::vector<int> &labels) {
  std::map<int, size_t> result;
  for (const int &l : labels) {
    if (l != 0) {
      ++result[l];
    }
  }
  return result;
}


/** Writes a grid of labels to a file of given file name.
 * @param filename the name of the desired output file (will be created)
 * @param labels vector of cluster labels on grid
 */
void writeToFile(const std::string &filename, const std::vector<int> &labels) {
  const size_t L = sqrt(labels.size());
  std::ofstream ofs(filename);

  for (size_t y = 0; y < L; y++) {
    for (size_t x = 0; x < L; x++) {
      ofs << labels[y * L + x] << " ";
    }
    ofs << '\n';
  } 
  ofs.close();
}
