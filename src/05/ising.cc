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
                                 const size_t, const size_t, URNG&,
                                 const int Jx=1, const int Jy=1);


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
  runMetropolisAndSaveResults("decoupled.txt", 1., L, T, rng, 1, 0);
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
  size_t here;
  int H = 0,
      S;

  for (size_t y = 1; y < L - 1; y++) {
    for (size_t x = 1; x < L - 1; x++) {
      here = y * L + x;
      S = sc[here];
      H += Jx * S * sc[here - 1];
      H += Jx * S * sc[here + 1];
      H += Jy * S * sc[here - L];
      H += Jy * S * sc[here + L];
    }
    // At this point we abuse the y variable to also run over the top and
    // bottom row as well as the left and right column.
    // This is also the point where we'd introduce different boundary
    // conditions. Here, we use periodical boundary conditions.
    // top
    here = y;
    S = sc[here];
    H += Jx * S * sc[here - 1];
    H += Jx * S * sc[here + 1];
    H += Jy * S * sc[here + (L - 1) * L];  // row "above" is bottom row
    H += Jy * S * sc[here + L];
    // bottom
    here = (L - 1) * L + y;
    S = sc[here];
    H += Jx * S * sc[here - 1];
    H += Jx * S * sc[here + 1];
    H += Jy * S * sc[here - L];
    H += Jy * S * sc[y];  // row "below" is top row
    // left
    here = y * L;
    S = sc[here];
    H += Jx * S * sc[here - 1 + L];  // column "to the left" is rightmost column
    H += Jx * S * sc[here + 1];
    H += Jy * S * sc[here + L];
    H += Jy * S * sc[here - L];
    // right
    here = (y + 1) * L - 1;
    S = sc[here];
    H += Jx * S * sc[here - 1];
    H += Jx * S * sc[here + 1 - L];  // colum "to the right" is leftmost column
    H += Jy * S * sc[here + L];
    H += Jy * S * sc[here - L];
  }
  // Also have to do the points in the corners.
  const size_t tl = 0,
               tr = L - 1,
               bl = (L - 1) * L,
               br = L * L - 1;
  H += sc[tl] * (Jy * sc[bl]     + Jx * sc[tl + 1] + Jy * sc[tl + L] + Jx * sc[tr]);  // top left
  H += sc[tr] * (Jy * sc[br]     + Jx * sc[tl]     + Jy * sc[tr + L] + Jx * sc[tr - 1]);  // top right
  H += sc[bl] * (Jy * sc[bl - L] + Jx * sc[bl + 1] + Jy * sc[tl]     + Jx * sc[br]);  // bottom left
  H += sc[br] * (Jy * sc[br - L] + Jx * sc[bl]     + Jy * sc[tr]     + Jx * sc[br - 1]);  // bottom right
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

  return 4 * sc[pos] * (Jy * top + Jx * right + Jy * bottom + Jx * left);
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
  URNG &rng,
  const int Jx,
  const int Jy
) {
  SpinConfiguration sc_u(L * L, 1);  // uniform, all up
  SpinConfiguration sc_r = randomConfiguration(L, rng);

  int H_u = calculateH(sc_u, Jx, Jy),
      H_r = calculateH(sc_r, Jx, Jy);
  
  RunningStats<double> E_u,
                       E_r;
  E_u.Push(H_u);
  E_r.Push(H_r);
  
  std::ofstream ofs(filename);
  ofs << 0 << ' ' << E_u.Mean() << ' ' << E_u.SEM()
           << ' ' << E_r.Mean() << ' ' << E_r.SEM() << '\n';
  for (size_t t = 1; t < T; t++) {
    H_u = metropolisStep(beta, H_u, sc_u, rng, Jx, Jy);
    H_r = metropolisStep(beta, H_r, sc_r, rng, Jx, Jy);
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
