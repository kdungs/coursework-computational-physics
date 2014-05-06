#include <iostream>
#include <random>
#include <vector>

template <typename Generator>
double MarkovChainVolume(size_t, size_t, Generator&);
double AbsSquared(const std::vector<double> &v);

int main(int main, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::cout << MarkovChainVolume(2, 1000, rng) << std::endl;
}

template <typename Generator>
double MarkovChainVolume(size_t dimension, size_t steps, Generator &g) {
  std::uniform_real_distribution<double> dist_dx(-1, 1);
  std::uniform_int_distribution<size_t> dist_pos(0, dimension - 1);
  std::vector<double> v(dimension);
  double mean = 0,
         dx;
  size_t pos;
  for (size_t i = 0; i < steps; i++) {  // actual MCMC
    pos = dist_pos(g);
    dx = dist_dx(g);
    v[pos] += dx;
    if (AbsSquared(v) > 1) {
      v[pos] -= dx;  // reject step, undo
    }
    mean += AbsSquared(v) / steps;
  }
  return mean;
}

double AbsSquared(const std::vector<double> &v) {
  return std::accumulate(std::begin(v), std::end(v), 0,
    [] (const double &acc, const double &x) {
      return acc + x * x;
    }
  );
}
