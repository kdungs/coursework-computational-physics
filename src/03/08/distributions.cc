#include <algorithm>
#include <iostream>
#include <random>

#include "boxmueller.hpp"

int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  BoxMuellerDist<double> bmd;

  for (int i=0; i<10000; i++) {
    std::cout << bmd(rng) << std::endl;
  }                         
}
