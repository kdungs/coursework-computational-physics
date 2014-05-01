#ifndef __CENTRALLIMIT_HPP__
#define __CENTRALLIMIT_HPP__

#include <cmath>
#include <limits>
#include <ostream>
#include <random>
#include <utility>


template <typename T=double>
class CentralLimitDist {
 private:
  T mean_,
    sigma_;
  const static size_t N = 100;  // Yeah, not so smart
 public:
  typedef T result_type;
  typedef std::pair<T, T> param_type;
  CentralLimitDist() : mean_(0), sigma_(1) {}
  CentralLimitDist(const T mean, const T sigma) : mean_(mean), sigma_(sigma) {}
  const param_type param() const { return {mean_, sigma_}; }
  void param(const param_type &p) {
    mean_ = p.first;
    sigma_ = p.second;
  }
  const T min() const { return std::numeric_limits<T>::min(); }
  const T max() const { return std::numeric_limits<T>::max(); }
  const bool operator==(const CentralLimitDist<T> &rhs) const {
    return mean_ == rhs.mean_ && sigma_ == rhs.sigma_;
  }
  const bool operator!=(const CentralLimitDist<T> &rhs) const {
    return mean_ != rhs.mean_ || sigma_ != rhs.sigma_;
  }
  void reset() const {}
  template <class Generator>
  T operator() (Generator &g) const {
    static std::uniform_real_distribution<T> uni(0, 1);
    auto rnd = std::bind(uni, g);
    T res(0);
    for (size_t i = 0; i < N; i++) {
      res += rnd();
    }
    res = std::sqrt(N) * (res / N - .5);
    res *= std::sqrt(12);  // compensate variance of uniform dist
    return mean_ + sigma_ * res;
  }
  template <class Generator>
  T operator() (Generator &g, const param_type &p) const {
    static std::uniform_real_distribution<T> uni(0, 1);
    auto rnd = std::bind(uni, g);
    T res(0);
    for (size_t i = 0; i < N; i++) {
      res += rnd();
    }
    res = std::sqrt(N) * (res / N - .5);
    res *= std::sqrt(12);  // compensate variance of uniform dist
    return p.first + p.second * res;
  }
};


template <typename T>
std::ostream &operator<<(std::ostream &out, const CentralLimitDist<T> &d) {
  auto p = d.param();
  out << p.first << ", " << p.second;
  return out;
}


template <typename T>
std::istream &operator>>(std::istream &in, CentralLimitDist<T> &d) {
  T mean,
    sigma;
  in >> mean >> sigma;
  d.param({mean, sigma});
  return in;
}



#endif  // __CENTRALLIMIT_HPP__
