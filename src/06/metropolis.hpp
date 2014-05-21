#ifndef __METROPOLIS_HPP__
#define __METROPOLIS_HPP__

#include <cmath>
#include <functional>
#include <random>
#include <vector>

typedef short Spin;
typedef std::vector<Spin> SpinConfiguration;
typedef std::function<double(const SpinConfiguration&,
                             const double)> QuantityCalculator;

template <typename URNG>
SpinConfiguration randomConfiguration(const size_t side_length, URNG &rng) {
  static std::uniform_real_distribution<double> dist(0, 1);
  SpinConfiguration sc(side_length * side_length);
  std::generate(std::begin(sc), std::end(sc), [&] () {
    if (dist(rng) < .5) {
      return 1;  // up
    }
    return -1;  // down
  });
  return sc;  // use return value optimisation
}

double calculateEnergy(
  const SpinConfiguration &sc,
  const double coupling_x=1,
  const double coupling_y=1
) {
  const size_t side_length = std::sqrt(sc.size());
  double energy = 0;
  for (size_t pos = 0; pos < sc.size(); pos++) {
    if ((pos + 1) % side_length == 0) {  // right edge
      energy += coupling_x * sc[pos] * sc[pos + 1 - side_length];
    } else {
      energy += coupling_x * sc[pos] * sc[pos + 1];
    }

    if ((pos + side_length) >= sc.size()) {  // bottom edge
      energy += coupling_y * sc[pos] * sc[pos % side_length];
    } else {
      energy += coupling_y * sc[pos] * sc[pos + side_length];
    }
  }
  return -energy;
}

double calculateEnergyDifference(
  const SpinConfiguration &sc,
  const size_t pos,
  const double coupling_x=1,
  const double coupling_y=1
) {
  const size_t side_length = std::sqrt();
}

template <typename URNG>
double metropolis(
  const double temperature,
  const size_t side_length,
  const unsigned long num_steps_burnin,
  const unsigned long num_sweeps,
  QuantityCalculator calculate_quantity,
  URNG &rng,
  const double coupling_x=1,
  const double coupling_y=1
) {
  SpinConfiguration sc = randomConfiguration(side_length, rng);
  double energy = calculateEnergy(sc);
  for (unsigned long i = 0; i < num_steps_burnin; i++) {
    energy = metropolisStep(temperature, energy, sc, rng, coupling_x,
                            coupling_y);
  }
}

#endif  // __METROPOLIS_HPP__
