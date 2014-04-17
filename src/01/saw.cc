/** @file saw.cc
 * Implement a self-avoiding (random) walk.
 */
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "point.h"

typedef std::vector<Point> Walk;
typedef std::mt19937 RandomEngine;

/** Check if a new point is legal in the context of a walk.
 * It is actually not necessary to check the whole vector. The position of the
 * new point is known and afterwards only (0, 0)s follow. This is clearly
 * something that could be optimised...
 * @param w the (partial) walk, that p is to be added to
 * @param p the new point
 * @return whether p is legal in the context of w
 */
bool IsLegalPoint(const Walk &w, const Point &p) {
  return std::find(std::begin(w), std::end(w), p) == std::end(w);
}

/** Construct a new self-avoiding random walk on a given vector of given size.
 * The function is called recursively until the vector is full.
 * @param step the current step
 * @param walk reference to a container in which the walk is stored
 * @param rand the random engine used for generating random numbers
 * @return true if a SAW was constructed, false if not
 */
bool SelfAvoidingWalk(const size_t step,  Walk &walk, RandomEngine &rand) {
  if (step == walk.size()) {
    return true;  // yay! we found a legal SAW
  }
  std::vector<Point> directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  std::shuffle(std::begin(directions), std::end(directions), rand);
  for (const Point &direction : directions) {
    Point new_point(walk[step - 1] + direction);
    if (IsLegalPoint(walk, new_point)) {
      walk[step] = walk[step - 1] + direction;
      if (SelfAvoidingWalk(step + 1, walk, rand)) {
        return true;  // we're done here and can go home
      }  // otherwise we have to try the next direction
    }
  }
  return false;  // if no direction succeeded, we return home shamefully
}

/** Calculate R_N^2 for a given walk.
 * @param the walk
 * @return R_N^2, the average cluster size squared
 */
double CalculateAverageClusterSizeSquared(const Walk &walk) {
  std::vector<Point> directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  Walk test_walk(walk.size());
  double walk_weight(1.0);
  std::for_each(std::begin(walk), std::end(walk),
                [&](const Point p){
                  double single_weight(0.0);
                  test_walk.push_back(p);
                  for(const Point &direction : directions) {
                    Point new_point(p + direction);
                    if (IsLegalPoint(test_walk, new_point)) { ++single_weight; };
                  }
                  walk_weight = walk_weight * single_weight > 0.0 ? walk_weight * single_weight : 1.0;
                });
  return 1.0 / (walk_weight * walk_weight) * walk.back().Squared();
}


int main(int argc, char *argv[]) {
  // set up random number generator
  std::random_device rd;
  std::mt19937 rng(rd());
  // calculate average cluster size for SAWs of varying lengths
  std::vector<double> RN2s(1000000);
  size_t N;
  double mean,
         sem;
  for (N = 20; N <= 60; N += 5) {
    Walk walk(N);
    std::generate(std::begin(RN2s), std::end(RN2s), [&]() {
      std::fill(std::begin(walk), std::end(walk), Point{0, 0});
      while(!SelfAvoidingWalk(1, walk, rng)) {}  // make sure we have a SAW
      return CalculateAverageClusterSizeSquared(walk);
    });
    mean = std::accumulate(std::begin(RN2s), std::end(RN2s), 0.0)
           / RN2s.size();
    sem = sqrt(std::accumulate(std::begin(RN2s), std::end(RN2s), 0.0,
                          [&](const double acc, const double x) {
                            return acc + (x - mean) * (x - mean); 
                          })) / RN2s.size();  // N(N-1) ~ N^2 here
    std::cout << N << " " << mean << " " << sem << std::endl;
  }
}
