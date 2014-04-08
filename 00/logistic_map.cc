#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#define NRUNS 100  // not so nice... use sth else than a define
typedef double number_t;  // in case we might want to use floats instead

/** Calculate next step of logistic map for given x_n.
 * x_{n+1} = r x_n (1 - x_n)
 */
number_t logistic_map(const number_t xn, const number_t r) noexcept {
  return r * xn * (1 - xn);
}

/** For a given x0 and r, calculate xN, the fixed point of the sequence.
 */
number_t calculate_xN(const number_t x0, const number_t r) noexcept {
  number_t xN = x0;
  for (size_t i=0; i<NRUNS; i++) {
    xN = logistic_map(xN, r);
  }
  return xN;
}

/** Write a set of numbers to a file.
 * We want this to take a copy of the vector, so computations can be performed
 * in parallel while writing to file
 */
void write_to_file(const number_t r, const std::vector<number_t> xs) {
  std::stringstream filename;
  filename << "./data/" << r << ".txt";
  std::ofstream ofs(filename.str());
  std::for_each(std::begin(xs), std::end(xs),
                [&](number_t const &x) { ofs << x << std::endl; });
  ofs.close();
}


int main(int argc, char *argv[]) {
  size_t len_xs = 1000;
  std::vector<number_t> xs(len_xs - 1);
  for (number_t r=0; r<4; r+=0.1) {
    // Fill xs with numbers from 0 to 1.
    std::iota(std::begin(xs), std::end(xs), 1);
    std::for_each(std::begin(xs), std::end(xs),
                  [&](number_t &x) {x /= len_xs; });
    // Calculate the fixed point for the whole range.
    std::for_each(std::begin(xs), std::end(xs),
                  [&](number_t &x) { x = calculate_xN(x, r); });
    // Save to file
    write_to_file(r, xs);
  }
}
