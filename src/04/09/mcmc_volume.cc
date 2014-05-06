/** @file mcmc_volume.cc
 * Markov Chain Monte Carlo for calculating volumes in high dimensions.
 */
#include <fstream>
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

#include "runningstats.hpp"


template <typename URNG>
std::tuple<double, double> MCMC_X2(size_t, size_t, URNG&);

template <typename URNG>
std::tuple<double, double> MCMC_Q(size_t, size_t, URNG&);


int main(int main, char *argv[]) {
  const size_t N = 100000;
  double mean, sem;
  
  std::random_device rd;
  std::mt19937 rng(rd());
  
  // (c)
  std::tie(mean, sem) = MCMC_X2(2, 1000000, rng);
  std::cout << mean << " ± " << sem << std::endl;

  // (d)
  std::ofstream ofs("Qs.txt");
  for (double d = 1; d < 200; ++d) {
    std::tie(mean, sem) = MCMC_Q(d, N, rng);
    ofs << mean << ' ' << sem << '\n';
  }
  ofs.close();
}


template <typename URNG>
std::tuple<double, double> MCMC_X2(size_t dim, size_t steps, URNG &g) {
  std::uniform_real_distribution<double> dist_dx(-1, 1);
  std::uniform_int_distribution<size_t> dist_pos(0, dim - 1);
  std::vector<double> v(dim);
  double X2 = 0,
         new_X2,
         rand_dx;
  size_t rand_pos;
  RunningStats<> rs;
  for (size_t i = 0; i < steps; i++) {
    rand_pos = dist_pos(g);
    rand_dx = dist_dx(g);
    new_X2 = X2 - v[rand_pos] * v[rand_pos]
           + (v[rand_pos] + rand_dx) * (v[rand_pos] + rand_dx);
    if (new_X2 < 1) {
      v[rand_pos] += rand_dx;
      X2 = new_X2;
    }
    rs.Push(X2);
  }
  return rs.MeanAndSEM();
}


template <typename URNG>
std::tuple<double, double> MCMC_Q(size_t dim, size_t steps, URNG &g) {
  std::uniform_real_distribution<double> dist_dx(-1, 1);
  std::uniform_int_distribution<size_t> dist_pos(0, dim - 1);
  std::vector<double> v(dim);
  double X2 = 0,
         new_X2,
         rand_dx,
         rand_h;
  size_t rand_pos;
  RunningStats<> rs;
  for (size_t i = 0; i < steps; i++) {
    rand_pos = dist_pos(g);
    rand_dx = dist_dx(g);
    new_X2 = X2 - v[rand_pos] * v[rand_pos]
           + (v[rand_pos] + rand_dx) * (v[rand_pos] + rand_dx);
    if (new_X2 < 1) {
      v[rand_pos] += rand_dx;
      X2 = new_X2;
    }
    rand_h = dist_dx(g);
    if (X2 + rand_h * rand_h < 1) {
      rs.Push(1);
    } else {
      rs.Push(0);
    }
  }
  return rs.MeanAndSEM();
}
