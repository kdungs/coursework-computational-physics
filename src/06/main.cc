#include <iostream>
#include <random>

#include "metropolis.hpp"
#include "runningstats.hpp"


struct HeatCapacityCalculator {
 private:
  const double T_sq_;
  const size_t L_sq_;
  RunningStats<> energy_,
                 energy_squared_;
 public:
  HeatCapacityCalculator(const double temperature, const size_t side_length)
   : T_sq_(temperature * temperature), L_sq_(side_length * side_length) {}
  double operator() (const SpinConfiguration &sc, const double hamiltonian) {
    energy_.Push(hamiltonian);
    energy_squared_.Push(hamiltonian * hamiltonian);
    const auto E = energy_.Mean();
    return 1. / T_sq_ / L_sq_ * (energy_squared_.Mean() - E * E);
  }
};

struct MagnetisationCalculator {
 private:
  RunningStats<> magnetisation;
 public:
  double operator() (const SpinConfiguration &sc, const double hamiltonian) {
    // TODO: Implement
    return 0;
  }
};


int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  const double T = 3;
  const size_t L = 100;
  auto calc_C = HeatCapacityCalculator(T, L);
  RunningStats<> C;
  for (int i = 0; i < 10; i++) {
    C.Push(metropolis(T, L, 1000, 1000, calc_C, rng));
  }
  std::cout << "C_\\nu = " << C.Mean() << " ± " << C.StdDev() << std::endl;
}
