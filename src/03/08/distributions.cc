#include <fstream>
#include <random>

#include "boxmueller.hpp"
#include "centrallimit.hpp"
#include "acceptreject.hpp"
#include "transformation.hpp"


int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  std::ofstream ofs_gaussian("gaussian.txt");
  std::ofstream ofs_sinxover2("sinxover2.txt");
  std::ofstream ofs_3xsquared("3xsquared.txt");
  BoxMuellerDist<> bm;
  CentralLimitDist<> cl;
  AcceptRejectSinxOver2Dist<> ar;
  Transformation3xSquaredDist<> tr;
  for (int i=0; i<100000; i++) {
    ofs_gaussian << bm(rng) << " " << cl(rng) << '\n';
    ofs_sinxover2 << ar(rng) << '\n';
    ofs_3xsquared << tr(rng) << '\n';
  }
  ofs_gaussian.close();
  ofs_sinxover2.close();
  ofs_3xsquared.close();
}
