#include <fstream>
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

template <typename Generator>
double MarkovChainVolume(size_t, size_t, Generator&);
double AbsSquared(const std::vector<double> &v);

int main(int main, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::vector<double> Qs(199);
  std::iota(std::begin(Qs), std::end(Qs), 1.0);
  auto f = std::bind(MarkovChainVolume<decltype(rng)>,
    std::placeholders::_1, 100000, rng);
  std::transform(std::begin(Qs), std::end(Qs), std::begin(Qs), f);
  std::ofstream ofs("Qs.txt");
  std::copy(std::begin(Qs), std::end(Qs),
    std::ostream_iterator<double>(ofs, "\n"));
  ofs.close();

  //std::cout << MarkovChainVolume(2, 100000, rng);
}

template <typename Generator>
double MarkovChainVolume(size_t dimension, size_t steps, Generator &g) {
  std::uniform_real_distribution<double> dist_dx(-1, 1);
  std::uniform_int_distribution<size_t> dist_pos(0, dimension - 1);
  std::vector<double> v(dimension);
  double Q = 0,
         dx,
         h;
  size_t pos;
  for (size_t i = 0; i < steps; i++) {  // actual MCMC
    pos = dist_pos(g);
    dx = dist_dx(g);
    h = dist_dx(g);
    v[pos] += dx;
    if (AbsSquared(v) >= 1) {
      v[pos] -= dx;  // reject step, undo
    }
    if (AbsSquared(v) + h * h < 1) {
      Q += 1.0 / steps;
    }
  }
  return Q;
}

double AbsSquared(const std::vector<double> &v) {
  return std::accumulate(std::begin(v), std::end(v), 0.0,
    [] (const double &acc, const double &x) {
      return acc + x * x;
    }
  );
}
