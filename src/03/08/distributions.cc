#include <fstream>
#include <random>

#include "boxmueller.hpp"
#include "centrallimit.hpp"
#include "acceptreject.hpp"


int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  std::ofstream ofs_gaussian("gaussian.txt");
  std::ofstream ofs_sinxover2("sinxover2.txt");
  BoxMuellerDist<> bm;
  CentralLimitDist<> cl;
  AcceptRejectSinxOver2Dist<> ar;
  for (int i=0; i<10000; i++) {
    ofs_gaussian << bm(rng) << " " << cl(rng) << '\n';
    ofs_sinxover2 << ar(rng) << '\n';
  }
  ofs_gaussian.close();
  ofs_sinxover2.close();


}
