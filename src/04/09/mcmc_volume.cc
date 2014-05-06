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

template <typename URNG>
double MCMC_X2(size_t, size_t, URNG&);

template <typename URNG>
double MCMC_Q(size_t, size_t, URNG&);


int main(int main, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());
  
  // (c)
  std::cout << MCMC_X2(2, 1000000, rng) << std::endl;

  // (d)
  const size_t N = 100000;
  std::vector<double> Qs(199);
  std::iota(std::begin(Qs), std::end(Qs), 1.0);
  auto f = std::bind(MCMC_Q<decltype(rng)>, std::placeholders::_1, N, rng);
  std::transform(std::begin(Qs), std::end(Qs), std::begin(Qs), f);
  std::ofstream ofs("Qs.txt");
  std::copy(std::begin(Qs), std::end(Qs),
    std::ostream_iterator<double>(ofs, "\n"));
  ofs.close();
}


template <typename URNG>
double MCMC_X2(size_t dim, size_t steps, URNG &g) {
  std::uniform_real_distribution<double> dist_dx(-1, 1);
  std::uniform_int_distribution<size_t> dist_pos(0, dim - 1);
  std::vector<double> v(dim);
  double mean_X2 = 0,
         X2 = 0,
         new_X2,
         rand_dx;
  size_t rand_pos;
  for (size_t i = 0; i < steps; i++) {
    rand_pos = dist_pos(g);
    rand_dx = dist_dx(g);
    new_X2 = X2 - v[rand_pos] * v[rand_pos]
           + (v[rand_pos] + rand_dx) * (v[rand_pos] + rand_dx);
    if (new_X2 < 1) {
      v[rand_pos] += rand_dx;
      X2 = new_X2;
    }
    mean_X2 += X2 / steps;
  }
  return mean_X2;
}


template <typename URNG>
double MCMC_Q(size_t dim, size_t steps, URNG &g) {
  std::uniform_real_distribution<double> dist_dx(-1, 1);
  std::uniform_int_distribution<size_t> dist_pos(0, dim - 1);
  std::vector<double> v(dim);
  double mean_Q = 0,
         X2 = 0,
         new_X2,
         rand_dx,
         rand_h;
  size_t rand_pos;
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
      mean_Q += 1.0 / steps;
    }
  }
  return mean_Q;
}
