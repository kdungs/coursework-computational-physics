/** @file.cc
 * π approximation
 */
#include <iostream>
#include <random>

/** Approximate π in a number of runs.
 * @param N the number of runs
 * @param rng the random number generator used
 * @return an approximation of π
 */
double ApproximatePi(const size_t N, std::mt19937 &rng) {
  static std::uniform_real_distribution<double> dist(0.0, 1.0);
  size_t i,
         n = 0;
  double x,
         y;
  for (i = 0; i < N; i++) {
    x = dist(rng);
    y = dist(rng);
    if (x * x + y * y <= 1) {
      ++n;
    }
  }
  return 4.0 * n / N;
}

/** Main. */
int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::cout << "π = " << ApproximatePi(10000000, rng) << std::endl;
}
