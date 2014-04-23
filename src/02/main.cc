#include <iostream>
#include <random>

#include "perculation.h"

int main(int argc, char *argv[]) {
  // set up RNG
  std::random_device rd;
  std::mt19937 rng(rd());

  const size_t L = 50;
  const double p = .5;
  auto grid = generateGrid(L, p, rng);
  auto labels = findClusters(grid);
  auto sizes = clusterSizes(labels);
  for (const auto &x : sizes) {
    std::cout << x.first << ": " << x.second << std::endl;
  }
  writeToFile("data.txt", labels);
  std::cerr << findPerculating(labels) << std::endl;
}
