/** @file saw.cc
 * Implement a self-avoiding (random) walk.
 */
#include <algorithm>
#include <iostream>
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
 * @param steps the maximum number of steps (also size of walk)
 * @param walk reference to a container in which the walk is stored
 * @param random_engine the random engine used for generating random numbers
 * @return true if a SAW was constructed, false if not
 */
bool SelfAvoidingWalk(const size_t step, const size_t steps, Walk &walk,
                      RandomEngine &random_engine) {
  if (step == steps) {
    return true;  // yay! we found a legal SAW
  }
  std::vector<Point> directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  std::shuffle(std::begin(directions), std::end(directions), random_engine);
  for (const Point &direction : directions) {
    Point new_point(walk[step - 1] + direction);
    if (IsLegalPoint(walk, new_point)) {
      walk[step] = walk[step - 1] + direction;
      if (SelfAvoidingWalk(step + 1, steps, walk, random_engine)) {
        return true;  // we're done here and can go home
      }  // otherwise we have to try the next direction
    }
  }
  return false;  // if no direction succeeded, we return home shamefully
}

int main(int argc, char *argv[]) {
  // Set up random number generator
  std::random_device rd;
  std::mt19937 rng(rd());
  // Run random walk
  std::vector<Point> walk(100);
  if (SelfAvoidingWalk(1, 100, walk, rng)) {
    std::for_each(std::begin(walk), std::end(walk), [](const Point &p) {
      p.Print();
    });
  }
}
