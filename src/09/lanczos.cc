#include <cmath>
#include <iostream>

#include <Eigen/Core>
#include <Eigen/Eigenvalues>


Eigen::MatrixXd MakeHamiltonian(const size_t N=10) {
  const size_t size = std::pow(2, N);
  Eigen::MatrixXd hm(size, size);
  for (size_t s = 0; s < size; s++) {
    for (size_t i = 0; i < N; i++) {
      const size_t j = (i + 1) % N,
                   mask_i = 1 << i,
                   mask_j = 1 << j;
      const bool bit_i = s & mask_i,
                 bit_j = s & mask_j;

      if (bit_i == bit_j) {
        hm(s, s) += 1;
      } else {
        hm(s, s) += -1;
        const size_t x = s ^ (mask_i | mask_j);
        hm(s, x) += 2;
      }
    }
  }
  hm *= -1. / 4;
  return hm;
}


int main(int argc, char *argv[]) {
  Eigen::MatrixXd hm = MakeHamiltonian();
  Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> ev(hm, false);
  std::cout << ev.eigenvalues() << std::endl;
}
