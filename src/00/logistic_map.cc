/** @file logistic_map.cc
 * Solution for sheet 0 exercise 2.
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

/** Define global variables in a nice way.
 */
enum Config {
  NUM_RUNS = 1000
};
/** Define a generic type for numbers that can be easily changed.
 * We might want to use floats at one point for example.
 */
typedef double Number;

/** Calculate next step of logistic map for given x_n.
 * x_{n+1} = r x_n (1 - x_n)
 *
 * @param xn the preceding result in the series
 * @param r the constant factor for the series
 * @return the next number in the series (x_{n+1})
 */
Number LogisticMap(const Number xn, const Number r) noexcept {
  return r * xn * (1 - xn);
}

/** For a given x0 and r, calculate xN, the fixed point of the sequence.
 *
 * @param x0 the starting value for which the fixed point is to be calculated
 * @param r the constant factor for the series
 * @return the fixpoint, determined after Config::NUM_RUNS runs
 */
Number CalculateFixpoint(const Number x0, const Number r) noexcept {
  Number xN = x0;
  for (size_t i = 0; i < Config::NUM_RUNS; i++) {
    xN = LogisticMap(xN, r);
  }
  return xN;
}

/** Write a set of numbers to a file.
 * We want this to take a copy of the vector, so computations can be performed
 * in parallel while writing to file.
 *
 * @param r the constant factor of the series - used for the file name
 * @param xs a vector of the fixed points for this r
 */
void WriteToFile(const Number r, const std::vector<Number> xs) {
  std::stringstream filename;
  filename << "./data/" << r << ".txt";
  std::ofstream ofs(filename.str());
  std::for_each(std::begin(xs), std::end(xs),
                [&](const Number &x) { ofs << x << std::endl; });
  ofs.close();
}

/** Main.
 */
int main(int argc, char *argv[]) {
  size_t len_xs = 1000;
  std::vector<Number> xs(len_xs - 1);
  for (Number r = 0; r < 4; r += 0.1) {
    std::iota(std::begin(xs), std::end(xs), 1);
    std::for_each(std::begin(xs), std::end(xs),
                  [&](Number &x) {x = CalculateFixpoint(x / len_xs, r); });
    WriteToFile(r, xs);
  }
}
