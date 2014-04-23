#include <algorithm>
#include <fstream>
#include <random>

#include "perculation.h"

int main(int argc, char *argv[]) {
  // set up RNG
  std::random_device rd;
  std::mt19937 rng(rd());

  // (b)
  const size_t N = 50;
  const std::vector<double> ps = {.1, .5, .9};
  std::for_each(std::begin(ps), std::end(ps), [&] (const double &p) {
    auto grid = generateGrid(N, p, rng);
    auto labels = findClusters(grid);
    writeToFile("data/50_" + std::to_string(p) + ".txt", labels);
  });
  
  // (c)
  const size_t R = 10000;
  const std::vector<size_t> Ls = {10, 50, 100};
  std::for_each(std::begin(Ls), std::end(Ls), [&] (const double &L) {
    std::vector<double> ps = {.1, .2, .3, .4, .5, .6, .7, .8, .9};
    std::vector<double> qs(ps.size());
    std::transform(std::begin(ps), std::end(ps), std::begin(qs),
      [&] (const double &p) {
        return simulateRelativePerculationRate(R, L, p, rng);
      }
    );
    std::ofstream ofs("data/q_" + std::to_string(L) + ".txt");
    for (size_t i = 0; i < ps.size(); i++) {
      ofs << ps[i] << " " << qs[i] << '\n';
    }
    ofs.close();
  });


}
