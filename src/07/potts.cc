#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

#include "union_find.hpp"

typedef unsigned short Spin;
typedef std::vector<Spin> SpinConfiguration;


template <typename URNG>
SpinConfiguration RandomPotts(const size_t L, const size_t q, URNG &rng) {
  std::uniform_int_distribution<Spin> dist_q(0, q - 1);
  auto random_q = std::bind(dist_q, rng);
  SpinConfiguration grid(L * L);
  std::generate(std::begin(grid), std::end(grid), random_q);
  return grid;
}

double calculateH(const SpinConfiguration &sc) {
  const size_t L = std::sqrt(sc.size());
  double H = 0;
  size_t right, bottom;

  for (size_t pos = 0; pos < sc.size(); pos++) {
    right = pos + 1;
    bottom = pos + L;
    if ((pos + 1) % L == 0) {
      right -= L;
    }
    if ((pos + L) >= L * L) {
      bottom = pos % L;
    }
    
    H += sc[H] == sc[bottom] ? 1 : 0;
    H += sc[H] == sc[right] ? 1 : 0;
  }

  return -H;
}

int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  SpinConfiguration grid = RandomPotts(100, 3, rng);
}
