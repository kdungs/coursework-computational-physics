#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

template <typename URNG>
std::vector<unsigned int> RandomPotts(const size_t L, const size_t q, URNG &rng) {
  std::uniform_int_distribution<unsigned int> dist_q(0, q - 1);
  auto random_q = std::bind(dist_q, rng);
  std::vector<unsigned int> grid(L * L);
  std::generate(std::begin(grid), std::end(grid), random_q);
  return grid;
}

double calculateH

int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  std::vector<unsigned int> grid = RandomPotts(100, 3, rng);
}
