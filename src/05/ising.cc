#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "runningstats.hpp"


typedef short Spin;
typedef std::vector<Spin> SpinConfiguration;

template <typename URNG>
SpinConfiguration randomConfiguration(const size_t, URNG&);

int calculateH(const SpinConfiguration&, const int, const int);
int updateH(const SpinConfiguration&, const size_t, const int, const int);

template <typename URNG>
double metropolisStep(const double, const int, SpinConfiguration&, URNG&,
                      const int Jx=1, const int Jy=1);

void saveToFile(const std::string&, const SpinConfiguration&);

template <typename URNG>
void runMetropolisAndSaveResults(const std::string&, const double,
                                 const size_t, const size_t, URNG&);

template <typename URNG>
std::tuple<double, double> metropolisMeasureEnergyAndSaveConfiguration(
  const std::string&,
  const double,
  const size_t,
  const size_t,
  const size_t,
  URNG&,
  const int Jx=1,
  const int Jy=0
);

int main(int argc, char *argv[]) {
  const size_t L = 100,
               T = 100000000;
  std::random_device rd;
  std::mt19937 rng(rd());
  
  // (a, b)
  std::map<double, std::string> kbTs{
    {1., "1.txt"},
    {1./2.25, "2.25.txt"},
    {1./3, "3.txt"},
    {1./50, "50.txt"},
    {1./1000, "1000.txt"}
  };
  for (const auto kbT : kbTs) {
    std::cout << "Working on " << kbT.second << std::endl;
    runMetropolisAndSaveResults(kbT.second, kbT.first, L, T, rng);
  }

  // (c)
  double mean, error;
  std::tie(mean, error) = metropolisMeasureEnergyAndSaveConfiguration(
    "decoupled.txt", 1, L, T, 10000, rng);
  std::cout << mean << " ± " << error << std::endl; 
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


int calculateH(const SpinConfiguration &sc, const int Jx=1, const int Jy=1) {
  const size_t L = std::sqrt(sc.size());
  int H = 0;
  for (size_t pos = 0; pos < sc.size(); pos++) {
    if ((pos + 1) % L == 0) {  // right edge
      H += Jx * sc[pos] * sc[pos + 1 - L];
    } else {
      H += Jx * sc[pos] * sc[pos + 1];
    }

    if ((pos + L) >= sc.size()) {  // bottom edge
      H += Jy * sc[pos] * sc[pos % L];
    } else {
      H += Jy * sc[pos] * sc[pos + L];
    }
  }
  return -H;
}


int updateH(
  const SpinConfiguration &sc,
  const size_t pos,
  const int Jx=1,
  const int Jy=1
) {
  const size_t L = std::sqrt(sc.size()),
               x = pos % L,
               y = pos / L;
  short top,
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

  return 2 * sc[pos] * (Jy * top + Jx * right + Jy * bottom + Jx * left);
}


template <typename URNG>
double metropolisStep(
 const double beta,
 const int H1,
 SpinConfiguration &sc,
 URNG &rng,
 const int Jx,
 const int Jy
) {
  static std::uniform_real_distribution<double> dist_p(0, 1);
  std::uniform_int_distribution<size_t> dist_pos(0, sc.size() - 1);
  size_t pos = dist_pos(rng);
  int diff = updateH(sc, pos, Jx, Jy);
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


template <typename URNG>
void runMetropolisAndSaveResults(
  const std::string &filename,
  const double beta,
  const size_t L,
  const size_t T,
  URNG &rng
) {
  SpinConfiguration sc_u(L * L, 1);  // uniform, all up
  SpinConfiguration sc_r = randomConfiguration(L, rng);

  int H_u = calculateH(sc_u),
      H_r = calculateH(sc_r);
  
  RunningStats<double> E_u,
                       E_r;
  E_u.Push(H_u);
  E_r.Push(H_r);
  
  std::ofstream ofs(filename);
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

  // Save final configurations
  saveToFile("sc_u_" + filename, sc_u);
  saveToFile("sc_r_" + filename, sc_r);
}

template <typename URNG>
std::tuple<double, double> metropolisMeasureEnergyAndSaveConfiguration(
  const std::string &filename,
  const double beta,
  const size_t L,
  const size_t T_burnin,
  const size_t T_measure,
  URNG &rng,
  const int Jx,
  const int Jy
) {
  SpinConfiguration sc = randomConfiguration(L, rng);
  int H = calculateH(sc, Jx, Jy);
  // burn-in
  for (size_t t = 0; t < T_burnin; t++) {
    H = metropolisStep(beta, H, sc, rng, Jx, Jy);
  }
  // actual measurement
  RunningStats<> E;
  for (size_t t = 0; t < T_measure; t++) {
    H = metropolisStep(beta, H, sc, rng, Jx, Jy);
    E.Push(H);
  }
  saveToFile(filename, sc);
  return E.MeanAndSEM();
}
