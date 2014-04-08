#include <algorithm>
#include <iostream>
#include <vector>

#define NRUNS 10  // not so nice... use sth else than a define
typedef double number_t;  // in case we might want to use floats instead

/** Calculate next step of logistic map for given x_n.
 * x_{n+1} = r x_n (1 - x_n)
 */
number_t logistic_map(const number_t xn, const number_t r) {
  return r * xn * (1 - xn);
}

/** For a given x0 and r, calculate xN, the fixed point of the sequence.
 */
number_t calculate_xN(const number_t x0, const number_t r) {
  number_t xN = x0;
  for (size_t i=0; i<NRUNS; i++) {
    xN = logistic_map(xN, r);
  }
  return xN;
}

int main(int argc, char *argv[]) {
  size_t len_xs = 1000;
  std::vector<number_t> xs(len_xs);
  for (number_t r=0; r<4; r+=0.01) {
    // Fill xs with numbers from 0 to 1.
    std::iota(std::begin(xs), std::end(xs), 1);
    std::for_each(std::begin(xs), std::end(xs),
                  [&](number_t &x) {x /= len_xs; });
    // Calculate the fixed point for the whole range.
    std::for_each(std::begin(xs), std::end(xs),
                  [&](number_t &x) { x = calculate_xN(x, r); });
  }
}
