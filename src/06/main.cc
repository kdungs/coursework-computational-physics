#include <iostream>
#include <random>

#include "metropolis.hpp"
#include "runningstats.hpp"

int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  const double T = 3;
  const size_t L = 100;
  auto calc_C = HeatCapacityCalculator(T, L);
  RunningStats<> C;
  for (int i = 0; i < 10; i++) {
    C.Push(metropolis(T, L, 100000, 1000, calc_C, rng));
  }
  std::cout << "C_\\nu = " << C.Mean() << " ± " << C.StdDev() << std::endl;
}
