#ifndef __METROPOLIS_HPP__
#define __METROPOLIS_HPP__

#include <algorithm>
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
  static std::uniform_int_distribution<Spin> dist(0, 1);
  SpinConfiguration sc(side_length * side_length);
  std::generate(std::begin(sc), std::end(sc), [&] () {
    if (dist(rng) == 1) {
      return 1;  // up
    }
    return -1;  // down
  });
  return sc;  // use return value optimisation
}


double calculateHamiltonian(
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


double calculateHamiltonianDifference(
  const SpinConfiguration &sc,
  const size_t pos,
  const double coupling_x=1,
  const double coupling_y=1
) {
const size_t L = std::sqrt(sc.size()),
               x = pos % L,
               y = pos / L;
  Spin top,
       right,
       bottom,
       left;
  if (y > 0) {
    top = sc[pos - L];
  } else {
    top = sc[x + (L - 1) * L];
  }
  if (x < L - 1) {
    right = sc[pos + 1];
  } else {
    right = sc[pos + 1 - L];
  }
  if (y < L - 1) {
    bottom = sc[pos + L];
  } else {
    bottom = sc[x];
  }
  if (x > 0) {
    left = sc[pos - 1];
  } else {
    left = sc[pos - 1 + L];
  }

  return 2 * sc[pos] * (coupling_y * top + coupling_x * right
                      + coupling_y * bottom + coupling_x * left);
}


template <typename URNG>
double metropolisStep(
  const double temperature,
  const double hamiltonian_before,
  const size_t pos,
  SpinConfiguration &sc,
  URNG &rng,
  const double coupling_x,
  const double coupling_y
) {
  static std::uniform_real_distribution<double> dist_p(0, 1);
  const double diff = calculateHamiltonianDifference(sc, pos, coupling_x,
                                                     coupling_y);
  if (diff > 0 && dist_p(rng) > std::exp(-1. / temperature * diff)) {
    return hamiltonian_before;  // reject flip
  }
  sc[pos] *= -1;
  return hamiltonian_before + diff;
}


template <typename URNG>
double metropolisRandomStep(
  const double temperature,
  const double hamiltonian_before,
  SpinConfiguration &sc,
  URNG &rng,
  const double coupling_x,
  const double coupling_y
) {
  std::uniform_int_distribution<size_t> dist_pos(0, sc.size() - 1);
  return metropolisStep(temperature, hamiltonian_before, dist_pos(rng), sc,
                        rng, coupling_x, coupling_y);
}


template <typename URNG>
double metropolisSweep(
  const double temperature,
  const double hamiltonian_before,
  SpinConfiguration &sc,
  URNG &rng,
  const double coupling_x,
  const double coupling_y
) {
  double hamiltonian = hamiltonian_before;
  for (size_t pos = 0; pos < sc.size(); pos++) {
    hamiltonian = metropolisStep(temperature, hamiltonian, pos, sc, rng,
                                 coupling_x, coupling_y);
  }
  return hamiltonian;
}


template <typename URNG>
double metropolis(
  const double temperature,
  const size_t side_length,
  const unsigned long num_sweeps_burnin,
  const unsigned long num_sweeps_measurement,
  QuantityCalculator calculate_quantity,
  URNG &rng,
  const double coupling_x=1,
  const double coupling_y=1
) {
  SpinConfiguration sc = randomConfiguration(side_length, rng);
  double hamiltonian = calculateHamiltonian(sc);
  for (unsigned long i = 0; i < num_sweeps_burnin; i++) {
    hamiltonian = metropolisSweep(temperature, hamiltonian, sc, rng,
                                  coupling_x, coupling_y);
  }
  double quantity = calculate_quantity(sc, hamiltonian);
  for (unsigned long i = 0; i < num_sweeps_measurement; i++) {
    hamiltonian = metropolisSweep(temperature, hamiltonian, sc, rng,
                                  coupling_x, coupling_y);
    quantity = calculate_quantity(sc, hamiltonian);
  }
  return quantity;
}

#endif  // __METROPOLIS_HPP__
