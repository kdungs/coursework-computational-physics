/** @file.cc
 * Fast (but memory consuming) C++11 implementation of π approximation
 */
#include <algorithm>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

typedef float Number;
typedef std::pair<Number, Number> Point;
typedef std::mt19937 RandomGenerator;

/** Approximate π in a number of runs.
 * @param N the number of runs
 * @param rng the random number generator used
 * @return an approximation of π
 */
Number ApproximatePi(const size_t N, RandomGenerator &rng) {
  std::uniform_real_distribution<Number> dist(0.0, 1.0);
  std::vector<Point> v(N);
  std::generate(std::begin(v), std::end(v), [&]() {
    return Point{dist(rng), dist(rng)};
  });
  size_t n_inside = std::count_if(std::begin(v), std::end(v),
    [](const Point &p) { return p.first * p.first + p.second * p.second <= 1; }
  );
  return 4.0 * n_inside / N;
}

/** Starting point.
 */
int main(int argc, char *argv[]) {
  std::random_device rd;
  RandomGenerator rng(rd());
  std::cout << "π = " << ApproximatePi(1000000, rng) << std::endl;
}
