#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "runningstats.hpp"


typedef short Spin;
typedef std::vector<Spin> SpinConfiguration;

template <typename URNG>
SpinConfiguration randomConfiguration(const size_t, URNG&);

int calculateH(const SpinConfiguration&);
int updateH(const SpinConfiguration&, const size_t);

template <typename URNG>
double metropolisStep(const double, const int, SpinConfiguration&, URNG&);

void saveToFile(const std::string&, const SpinConfiguration&);


int main(int argc, char *argv[]) {
  const size_t L = 100,
               T = 100000000;
  const double beta = 1./3;
  std::random_device rd;
  std::mt19937 rng(rd());
  
  SpinConfiguration sc_u(L * L, 1);  // uniform, all up
  SpinConfiguration sc_r = randomConfiguration(L, rng);

  int H_u = calculateH(sc_u),
      H_r = calculateH(sc_r);

  RunningStats<double> E_u,
                       E_r;
  E_u.Push(H_u);
  E_r.Push(H_r);

  std::ofstream ofs("E.txt");
  ofs << 0 << ' ' << E_u.Mean() << ' ' << E_u.SEM()
           << ' ' << E_r.Mean() << ' ' << E_r.SEM() << '\n';
  for (size_t t = 1; t < T; t++) {
    H_u = metropolisStep(beta, H_u, sc_u, rng);
    H_r = metropolisStep(beta, H_r, sc_r, rng);
    E_u.Push(H_u);
    E_r.Push(H_r);
    if (t % 1000 == 0) {
      ofs << t << ' ' << E_u.Mean() << ' ' << E_u.SEM()
               << ' ' << E_r.Mean() << ' ' << E_r.SEM() << '\n';
    }
  }
  ofs.close();
}


template <typename URNG>
SpinConfiguration randomConfiguration(const size_t L, URNG &rng) {
  static std::uniform_real_distribution<double> dist(0, 1);

  SpinConfiguration sc(L * L);
  std::generate(std::begin(sc), std::end(sc), [&] () {
    if (dist(rng) < .5) {
      return 1;  // up
    }
    return -1;  // down
  });
  return sc;  // use return value optimisation
}


int calculateH(const SpinConfiguration &sc) {
  const size_t L = std::sqrt(sc.size());
  size_t here;
  int H = 0,
      S;

  for (size_t y = 1; y < L - 1; y++) {
    for (size_t x = 1; x < L - 1; x++) {
      here = y * L + x;
      S = sc[here];
      H += S * sc[here - 1];
      H += S * sc[here + 1];
      H += S * sc[here - L];
      H += S * sc[here + L];
    }
    // At this point we abuse the y variable to also run over the top and
    // bottom row as well as the left and right column.
    // This is also the point where we'd introduce different boundary
    // conditions. At this point, the points at the edges simply have less
    // neighbours to consider.
    // top
    here = y;
    S = sc[here];
    H += S * sc[here - 1];
    H += S * sc[here + 1];
    H += S * sc[here + L];
    // bottom
    here = (L - 1) * L + y;
    S = sc[here];
    H += S * sc[here - 1];
    H += S * sc[here + 1];
    H += S * sc[here - L];
    // left
    here = y * L;
    S = sc[here];
    H += S * sc[here + 1];
    H += S * sc[here + L];
    H += S * sc[here - L];
    // right
    here = (y + 1) * L - 1;
    S = sc[here];
    H += S * sc[here - 1];
    H += S * sc[here + L];
    H += S * sc[here - L];
  }
  // Also have to do the points in the corners.
  H += sc[0] * sc[1] + sc[0] * sc[L];
  H += sc[L - 1] * sc[L - 2] + sc[L - 1] * sc[2 * L - 1];
  H += sc[(L - 1) * L] * sc[(L - 1) * L + 1]
     + sc[(L - 1) * L] * sc[(L - 2) * L];
  H += sc[L * L - 1] * sc[L * L - 2] + sc[L * L - 1] * sc[(L - 1) * L - 1];

  return -H;
}


int updateH(const SpinConfiguration &sc, const size_t pos) {
  const size_t L = std::sqrt(sc.size()),
               x = pos % L,
               y = pos / L;

  int before = 0,
      after = 0;
  
  if (x > 0) {
    before += sc[pos] * sc[pos - 1];
    after += -sc[pos] * sc[pos - 1];
  }
  if (x < L - 1) {
    before += sc[pos] * sc[pos + 1];
    after += -sc[pos] * sc[pos + 1];
  }
  if (y > 0) {
    before += sc[pos] * sc[pos - L];
    after += -sc[pos] * sc[pos - L];
  }
  if (y < L - 1) {
    before += sc[pos] * sc[pos + L];
    after += -sc[pos] * sc[pos + L];
  }
  return -2 * (after - before);
}


template <typename URNG>
double metropolisStep(
 const double beta,
 const int H1,
 SpinConfiguration &sc,
 URNG &rng
) {
  static std::uniform_real_distribution<double> dist_p(0, 1);

  std::uniform_int_distribution<size_t> dist_pos(0, sc.size() - 1);
  size_t pos = dist_pos(rng);
  int diff = updateH(sc, pos);
  sc[pos] *= -1;
  if (diff > 0 && dist_p(rng) > std::exp(-beta * diff)) {
    sc[pos] *= -1;  // reject, revert flip
    return H1;
  }
  return H1 + diff;
}


void saveToFile(const std::string &filename, const SpinConfiguration &sc) {
  const size_t L = std::sqrt(sc.size());
  std::ofstream ofs(filename);
  for (size_t y = 0; y < L; y++) {
    for (size_t x = 0; x < L; x++) {
      ofs << sc[y * L + x] << ' ';
    }
    ofs << '\n';
  }
  ofs.close();
}
