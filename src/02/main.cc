#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

#include "percolation.h"

using namespace std::chrono;

int main(int argc, char *argv[]) {
  // set up RNG
  std::random_device rd;
  std::mt19937 rng(rd());

  // (b)
  std::cout << "(b)" << std::endl;
  const size_t N = 50;
  const std::vector<double> ps = {.1, .5, .9};
  std::for_each(std::begin(ps), std::end(ps), [&] (const double &p) {
    auto grid = generateGrid(N, p, rng);
    auto labels = findClusters(grid);
    writeToFile("data/50_" + std::to_string(p) + ".txt", labels);
  });
  
  // (c)
  std::cout << "(c)" << std::endl;
  const size_t R = 1000;
  const std::vector<size_t> Ls = {10, 50, 100};
  std::for_each(std::begin(Ls), std::end(Ls), [&] (const double &L) {
    std::vector<double> ps = {.05, .1, .15, .2, .25, .3, .35, .4, .45, .5, .55,
                              .6, .65, .7, .75, .8, .85, .9, .95};
    std::vector<double> qs(ps.size());
    std::transform(std::begin(ps), std::end(ps), std::begin(qs),
      [&] (const double &p) {
        std::cout << "L = " << L << ", p = " << p;
        steady_clock::time_point start = steady_clock::now();
        auto r = simulateRelativePercolationRate(R, L, p, rng);
        steady_clock::time_point end = steady_clock::now();
        milliseconds t = duration_cast<milliseconds>(end - start);
        std::cout << " took " << t.count() << " ms" << std::endl;
        return r;
      }
    );
    std::ofstream ofs("data/q_" + std::to_string(L) + ".txt");
    for (size_t i = 0; i < ps.size(); i++) {
      ofs << ps[i] << " " << qs[i] << '\n';
    }
    ofs.close();
  });
  

  // (d) but smarter
  //steady_clock::time_point start = steady_clock::now();
  //double pC = approximatePC(100, 1000, 0.001, rng);
  //steady_clock::time_point end = steady_clock::now();
  //seconds t = duration_cast<seconds>(end - start);
  //std::cout << "p_c = " << pC << " (took " << t.count() << " s)" << std::endl;
}
