#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

enum Spin {
  UP = 1,
  DOWN = -1
};
typedef std::vector<Spin> SpinConfiguration;


template <typename URNG>
SpinConfiguration randomConfiguration(const size_t, URNG&);

int calculateH(const SpinConfiguration&);


int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  SpinConfiguration sc = randomConfiguration(100, rng);
}


template <typename URNG>
SpinConfiguration randomConfiguration(const size_t L, URNG &rng) {
  static std::uniform_real_distribution<double> dist(0, 1);

  SpinConfiguration sc(L * L);
  std::generate(std::begin(sc), std::end(sc), [&] () {
    if (dist(rng) < .5) {
      return Spin::UP;
    }
    return Spin::DOWN;
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

  return H;
}
