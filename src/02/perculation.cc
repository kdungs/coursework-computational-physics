#include <algorithm>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

typedef std::pair<unsigned int, unsigned int> Point;

int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  const unsigned int L = 100;
  const double p = .5;
  unsigned int x,
               y,
               n = 0;
  std::vector<Point> points(L * L);
  for (x = 0; x < L; x++) {
    for (y = 0; y < L; y++) {
      if (dist(rng) > p) {
        points[n] = {x, y};
        ++n;
      }
    }
  }
  points.resize(n);
  std::vector<std::set<const Point&>> clusters;

}
