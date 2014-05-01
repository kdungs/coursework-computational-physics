#include <algorithm>
#include <iostream>
#include <random>

#include "boxmueller.hpp"
#include "centrallimit.hpp"

int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  BoxMuellerDist<> bmd;
  CentralLimitDist<> cl;

  for (int i=0; i<10000; i++) {
    std::cout << bmd(rng) << " " << cl(rng) << std::endl;
  }                         
}
