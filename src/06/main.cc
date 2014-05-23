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
    magnetisation_.Push(std::abs(std::accumulate(std::begin(sc), std::end(sc),
      0.0, [] (const double acc, const Spin &s) {
        return acc + s;
      }
    )));
    return magnetisation_.Mean();
  }
  double StdDev() {
    return magnetisation_.StdDev();
  }
};


struct BinderCumulantCalculator {
 private:
  RunningStats<> M_2_,
                 M_4_,
                 bc_;
 public:
  double operator() (const SpinConfiguration &sc, const double hamiltonian) {
    const double m = std::abs(std::accumulate(std::begin(sc), std::end(sc),
      0.0, [] (const double acc, const Spin &s) {
        return acc + s;
      }
    ));
    M_2_.Push(m * m);
    M_4_.Push(m * m * m * m);
    bc_.Push(1 - M_4_.Mean() / 3. / M_2_.Mean() / M_2_.Mean());
    return bc_.Mean();
  }
  double StdDev() {
    return bc_.StdDev();
  }
};


int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  // (a, b)
  for (size_t L = 25; L <= 100; L += 25) {
    std::ofstream ofs_C("critical_temperature_" + std::to_string(L) + ".txt");
    std::ofstream ofs_M("magnetisation_" + std::to_string(L) + ".txt");
    std::ofstream ofs_B("binder_" + std::to_string(L) + ".txt");
    for (double T = 2; T < 3; T += .02) {
      MagnetisationCalculator calc_M;
      BinderCumulantCalculator calc_B;
      ofs_C << T << ' '
            << metropolis(T, L, 1000, 1000, HeatCapacityCalculator(T, L), rng)
            << '\n';
      ofs_M << T << ' '
            << metropolis(T, L, 1000, 1000, calc_M, rng)
            << ' ' << calc_M.StdDev() << '\n';
      ofs_B << T << ' '
            << metropolis(T, L, 1000, 1000, calc_B, rng)
            << ' ' << calc_B.StdDev() << '\n';  // does not work so well
      // Instead of running three times, one could change metropolis() to
      // accept a vector/tuple/whatever of calculators and return a sth of
      // results or change the interface in such a way that we obtain the
      // calculators result in the same way we obtain the variance. However,
      // since time is short, this won't be implemented...
    }
    ofs_C.close();
    ofs_M.close();
    ofs_B.close();
  }
}
