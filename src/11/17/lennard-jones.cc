#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "Eigen/Eigen"

typedef Eigen::Vector2d Vec2d;

class System {
 private:
  bool _initialised;
  const size_t _N;
  std::vector<Vec2d> _r,
                     _v;
 public:
  System(const size_t N) : _initialised(false), _N(N), _r(N), _v(N) {}
  
  template <typename URNG>
  void initialise(URNG &rng, const double scale_v=1) {
    const size_t L = std::sqrt(_N);
    std::uniform_real_distribution<> dist_v(-scale_v, scale_v);
    // Fill position vectors
    for (size_t m = 0; m < L; m++) {
      for (size_t n = 0; n < L; n++) {
        _r[n + m * L] = {1. / 8 * (1 + 2 * n) * L,
                         1. / 8 * (1 + 2 * m) * L};
      }
    }
    // Generate velocity vectors and adjust them so that the cm velocity is 0
    std::generate(std::begin(_v), std::end(_v),
      [&] () {
        return Vec2d{dist_v(rng), dist_v(rng)};
      });
    const Vec2d mean_v = std::accumulate(
      std::begin(_v),
      std::end(_v),
      Vec2d{0.0, 0.0},
      [&] (const Vec2d &mean, const Vec2d &v) {
        return mean + v / _N;
      });
    std::transform(std::begin(_v), std::end(_v), std::begin(_v),
      [&] (const Vec2d &v) {
        return v - mean_v;
      });
    
    _initialised = true;
  }

  void print() const {
    std::cout << "r: ";
    std::copy(std::begin(_r), std::end(_r),
              std::ostream_iterator<Vec2d>(std::cout, ",\n"));
    std::cout << "v: ";
    std::copy(std::begin(_v), std::end(_v),
              std::ostream_iterator<Vec2d>(std::cout, ",\n"));
  }

  void timeStep() {
    std::vector<Vec2d> r_before(_r),
                       v_before(_v);

    // TODO: Verlet integration etc.
 
  }
};

int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  System s(16);
  s.initialise(rng);
  s.print();
}
