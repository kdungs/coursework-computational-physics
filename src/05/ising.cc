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
    // conditions. Here, we use periodical boundary conditions.
    // top
    here = y;
    S = sc[here];
    H += S * sc[here - 1];
    H += S * sc[here + 1];
    H += S * sc[here + (L - 1) * L];  // row "above" is bottom row
    H += S * sc[here + L];
    // bottom
    here = (L - 1) * L + y;
    S = sc[here];
    H += S * sc[here - 1];
    H += S * sc[here + 1];
    H += S * sc[here - L];
    H += S * sc[y];  // row "below" is top row
    // left
    here = y * L;
    S = sc[here];
    H += S * sc[here - 1 + L];  // column "to the left" is rightmost column
    H += S * sc[here + 1];
    H += S * sc[here + L];
    H += S * sc[here - L];
    // right
    here = (y + 1) * L - 1;
    S = sc[here];
    H += S * sc[here - 1];
    H += S * sc[here + 1 - L];  // colum "to the right" is leftmost column
    H += S * sc[here + L];
    H += S * sc[here - L];
  }
  // Also have to do the points in the corners.
  const size_t tl = 0,
               tr = L - 1,
               bl = (L - 1) * L,
               br = L * L - 1;
  H += sc[tl] * (sc[bl] + sc[tl + 1] + sc[tl + L] + sc[tr]);  // top left
  H += sc[tr] * (sc[br] + sc[tl] + sc[tr + L] + sc[tr - 1]);  // top right
  H += sc[bl] * (sc[bl - L] + sc[bl + 1] + sc[tl] + sc[br]);  // bottom left
  H += sc[br] * (sc[br - L] + sc[bl] + sc[tr] + sc[br - 1]);  // bottom right
  return -H;
}


int updateH(const SpinConfiguration &sc, const size_t pos) {
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

  return 4 * sc[pos] * (top + right + bottom + left);
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
