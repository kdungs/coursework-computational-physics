#include <algorithm>
#include <fstream>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "runningstats.hpp"


typedef short Spin;
typedef std::vector<Spin> SpinConfiguration;

template <typename URNG>
SpinConfiguration randomConfiguration(const size_t, URNG&);

int calculateH(const SpinConfiguration&);

template <typename URNG>
double metropolisStep(const double, SpinConfiguration&, URNG&);

void saveToFile(const std::string&, const SpinConfiguration&);


int main(int argc, char *argv[]) {
  const size_t L = 100,
               T = 1000000;
  const double beta = 1./3;
  std::random_device rd;
  std::mt19937 rng(rd());

  SpinConfiguration sc_u(L * L, 1);  // uniform, all up
  SpinConfiguration sc_r = randomConfiguration(L, rng);

  RunningStats<double> E_u,
                       E_r;
  E_u.Push(calculateH(sc_u));
  E_r.Push(calculateH(sc_r));

  std::ofstream ofs("E.txt");
  ofs << 0 << ' ' << E_u.Mean() << ' ' << E_u.SEM()
           << ' ' << E_r.Mean() << ' ' << E_r.SEM() << '\n';
  for (size_t t = 1; t < T; t++) {
    E_u.Push(metropolisStep(beta, sc_u, rng));
    E_r.Push(metropolisStep(beta, sc_r, rng));
    if (t % 100 == 0) {
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


template <typename URNG>
double metropolisStep(const double beta, SpinConfiguration &sc, URNG &rng) {
  static std::uniform_real_distribution<double> dist_p(0, 1);

  std::uniform_int_distribution<size_t> dist_pos(0, sc.size() - 1);
  size_t pos = dist_pos(rng);
  double H1 = calculateH(sc);
  sc[pos] *= -1;
  double H2 = calculateH(sc);
  if ((H2 - H1) > 0 && dist_p(rng) > std::exp(-beta * (H2 - H1))) {
    sc[pos] *= -1;  // reject, revert flip
    return H1;
  }
  return H2;
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
