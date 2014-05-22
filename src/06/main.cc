#include <iostream>
#include <random>

#include "metropolis.hpp"
#include "runningstats.hpp"


struct HeatCapacityCalculator {
 private:
  const double T_sq_;
  const size_t L_sq_;
  RunningStats<> energy_;
 public:
  HeatCapacityCalculator(const double temperature, const size_t side_length)
   : T_sq_(temperature * temperature), L_sq_(side_length * side_length) {}
  double operator() (const SpinConfiguration &sc, const double hamiltonian) {
    energy_.Push(hamiltonian);
    return 1. / T_sq_ / L_sq_ * energy_.Variance();
  }
};

struct MagnetisationCalculator {
 private:
  RunningStats<> magnetisation_;
 public:
  double operator() (const SpinConfiguration &sc, const double hamiltonian) {
    // We could update but meh.
    double m = std::abs(std::accumulate(std::begin(sc), std::end(sc), 0.0,
      [] (const double acc, const Spin &s) {
        return acc + s;
      }
    ));
    magnetisation_.Push(m);
    return m;
  }
};


int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  //const double T = 1;
  //const size_t L = 100;
  //HeatCapacityCalculator calc_C(T, L);
  //MagnetisationCalculator calc_M;
  //RunningStats<> C,
  //               M;
  //for (int i = 0; i < 10; i++) {
  //  C.Push(metropolis(T, L, 1000, 1000, calc_C, rng));
  //  M.Push(metropolis(T, L, 1000, 1000, calc_M, rng, 1, 1,
  //                    "m_" + std::to_string(i) + ".txt"));
  //}
  //std::cout << "C_v = " << C.Mean() << " ± " << C.StdDev() << std::endl;
  //std::cout << "\\lange M\\rangle = " << M.Mean() << " ± " << M.SEM()
  //          << std::endl;


  // (a)
  for (size_t L = 25; L <= 100; L += 25) {
    std::ofstream ofs("critical_temperature_" + std::to_string(L) + ".txt");
    for (double T = 2; T < 3; T += .02) {
      ofs << T << ' '
          << metropolis(T, L, 1000, 1000, HeatCapacityCalculator(T, L), rng)
          << '\n';
    }
    ofs.close();
  }
}
