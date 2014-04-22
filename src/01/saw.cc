/** @file saw.cc
 * Implement a self-avoiding (random) walk.
 */
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

#include "point.h"


double WeightedMean(const std::vector<std::pair<double, double>>&);

double SAW(const size_t, std::vector<Point>&, std::mt19937&);


/** Calculate 〈R_N^2〉for N=20..60 on 10000 clusters.
 * Results are written to a file named data.txt.
 */
int main(int argc, char *argv[]) {
  // set up random number generator
  std::random_device rd;
  std::mt19937 rng(rd());

  std::ofstream ofs("data.txt");

  std::vector<std::pair<double, double>> RNsq(10000);
  size_t N;
  for (N = 20; N <= 60; N += 5) {
    std::vector<Point> walk(N);
    std::generate(
      std::begin(RNsq),
      std::end(RNsq),
      [&] () {
        double w = SAW(1, walk, rng);
        return std::make_pair(walk[N - 1].Squared(), w);
      }
    );
    ofs << N << " " << WeightedMean(RNsq) << std::endl;
  }

  ofs.close();
}


/** Calculate weighted mean of a given vector of values and weights.
 * @param v vector<value, weight>
 * @return the weighted mean of the given vector
 */
double WeightedMean(const std::vector<std::pair<double, double>> &v) {
  double sum_w = std::accumulate(  // sum of weights - use for normalisation
    std::begin(v),
    std::end(v),
    0.0,
    [] (const double acc, const std::pair<double, double> &x) {
      return acc + x.second;
    }
  );
  double mean = std::accumulate(
    std::begin(v),
    std::end(v),
    0.0,
    [&] (const double acc, const std::pair<double, double> &x) {
      return acc + x.first * x.second / sum_w;
    }
  );
  return mean;
}

/** Construct a new self-avoiding random walk on a given vector of given size.
 * The function is called recursively until the vector is full.
 * Use step=1 for starting value.
 * @param step the current step
 * @param walk reference to a container in which the walk is stored
 * @param rng the random number generator used for generating random numbers
 * @return weight of the SAW (not sure if right weight, though)
 */
double SAW(const size_t step, std::vector<Point> &walk, std::mt19937 &rng) {
  const Point last = walk[step - 1];
  if (step == walk.size()) {
    return 1;
  }
  // find all legal neighbours
  std::vector<Point> neighbours{
    last + Point{0, 1},
    last + Point{1, 0},
    last + Point{0, -1},
    last + Point{-1, 0}
  };
  const auto iter_to_last = std::begin(walk) + step;
  const auto it = std::remove_if(
    std::begin(neighbours),
    std::end(neighbours),
    [&] (const Point &p) {
      return std::find(std::begin(walk), iter_to_last, p) != iter_to_last;
    }
  );
  neighbours.resize(std::distance(std::begin(neighbours), it));
  // shuffle legal neighbours and try to build saw based on them in order
  std::shuffle(std::begin(neighbours), std::end(neighbours), rng);
  for (const auto &p : neighbours) {
    walk[step] = p;
    auto future = SAW(step + 1, walk, rng);
    if (future > 0) {  // we've found a legal SAW
      return future * neighbours.size();  // is this still right for
                                          // backtracking or does it only
                                          // apply to Rosenbluth
    }
  }
  // no legal SAW was found; return home shamefully
  return 0;  
}
