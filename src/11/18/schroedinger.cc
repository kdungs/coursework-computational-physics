#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#include "Eigen/Eigen"
#include "Eigen/LU"


Eigen::MatrixXd discreteHamiltonianMatrix(const std::vector<double> &xi,
                                          const double delta_xi) {
  const size_t N = xi.size();
  const double one_over_delta_xi_squared = 1. / (delta_xi * delta_xi);
  Eigen::MatrixXd H(N, N);
  
  H(0, 0) = 2 * one_over_delta_xi_squared + xi[0] * xi[0];
  H(0, 1) = -one_over_delta_xi_squared;
  for (size_t n = 1; n < (N - 1); n++) {
    H(n, n - 1) = -one_over_delta_xi_squared;
    H(n, n) = 2 * one_over_delta_xi_squared + xi[n] * xi[n];
    H(n, n + 1) = -one_over_delta_xi_squared;
  }
  H(N - 1, N - 2) = -one_over_delta_xi_squared;
  H(N - 1, N - 1) = 2 * one_over_delta_xi_squared + xi[N - 1] * xi[N - 1];

  return H;
}


Eigen::MatrixXcd timeEvolutionOperator(const Eigen::MatrixXd &H,
                                       const double delta_t) {
  const std::complex<double> i2{0., .5};
  const Eigen::MatrixXcd id(Eigen::MatrixXcd::Identity(H.cols(), H.rows()));
  const Eigen::MatrixXcd Sl(id + i2 * H * delta_t);
  const Eigen::MatrixXcd Sr(id - i2 * H * delta_t);
  const Eigen::PartialPivLU<Eigen::MatrixXcd> luSl(Sl);
  return luSl.solve(Sr);
}


double gaussian(const double x, const double mu = 0., const double sigma = 1.) {
  return std::pow(1. / (2. * M_PI * sigma), .25) *
         std::exp(-1. * (x - mu) * (x - mu) / (4. * sigma));
}


int main(int argc, char *argv[]) {
  const size_t N = 201;
  const double offset = -100., delta_xi = .1, xi0 = 1., sigma = 1.,
               delta_t = .01;

  // Initialise ξ and ψ.
  std::vector<double> xi(N);
  std::iota(std::begin(xi), std::end(xi), offset);
  std::transform(std::begin(xi), std::end(xi), std::begin(xi),
                 [&](const double x) { return x * delta_xi; });
  std::vector<double> psi(N);
  std::transform(std::begin(xi), std::end(xi), std::begin(psi),
                 [&](const double x) { return gaussian(x, xi0, sigma); });
  // Convert ψ to be used with Eigen
  Eigen::Map<Eigen::VectorXd> psi_0(psi.data(), psi.size());

  const auto H = discreteHamiltonianMatrix(xi, delta_xi);
  const auto S = timeEvolutionOperator(H, delta_t);

  std::ofstream ofs("data/xi.txt");
  //std::copy(std::begin(xi), std::end(xi),
  //          std::ostream_iterator<double>(ofs, "\n"));
  for (const auto &x : xi) {
    ofs << x << '\n';
  }
  ofs.close();

  Eigen::VectorXcd psi_n(std::complex<double>{1, 0} * psi_0.normalized());
  for (double t = 0; t < 10.; t += delta_t) {
    ofs.open("data/psi_" + std::to_string(t) + ".txt");
    ofs << psi_n.real().cwiseProduct(psi_n.real()) +
               psi_n.imag().cwiseProduct(psi_n.imag()) << std::endl;
    ofs.close();

    psi_n = (S * psi_n).normalized();
  } 
}
