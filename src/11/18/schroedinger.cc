#include <complex>
#include <iostream>
#include <typeinfo>

#include "Eigen/Eigen"

double discreteHamiltonian(
  const int n,
  const int m,
  const double dxi=0.1
) {
  const int dnm = n == m,
            dnmp = n == (m + 1),
            dnmm = n == (m - 1);
  const double dxisq = dxi * dxi,
               nsq = n * n;

  return -1. / dxisq * (dnmm + dnmp - 2 * dnm) + dxisq * nsq * dnm;
}

Eigen::MatrixXd generateDiscreteHamiltonianMatrix(
  const size_t N=201,
  const int offset=100,
  const double de=0.1
) {
  Eigen::MatrixXd H(N, N);
  for (size_t j = 0; j < N; j++) {
    for (size_t i = 0; i < N; i++) {
      H(j, i) = discreteHamiltonian(j - offset, i - offset, de);
    }
  }
  return H;
}

Eigen::MatrixXcd calculateTimeEvolutionOperator(const Eigen::MatrixXd H,
                                               const double dt=.01) {
  const std::complex<double> i2{0, .5};
  const auto S = Eigen::MatrixXcd::Identity(H.cols(), H.rows()) + i2 * H * dt;
  const auto Sinv = S.inverse();
  return Sinv * S;
}

Eigen::VectorXd generateDiscreteGaussianPackage() {
  Eigen::VectorXd psi(1);

  return psi;
}

int main(int argc, char *argv[]) {
  auto H = generateDiscreteHamiltonianMatrix(11, 5);
  auto S = calculateTimeEvolutionOperator(H);

  std::cout << "H: " << H.rows() << ", " << H.cols() << std::endl;
  std::cout << "S: " << S.rows() << ", " << S.cols() << std::endl;

  std::cout << H << std::endl;
}
