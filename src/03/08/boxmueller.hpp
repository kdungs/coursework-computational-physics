#ifndef __BOXMUELLER_H__
#define __BOXMUELLER_H__

#include <cmath>
#include <limits>
#include <ostream>
#include <random>
#include <utility>


template <typename T=double>
class BoxMuellerDist {
 private:
   T mean_,
     sigma_;
 public:
  typedef T result_type;
  typedef std::pair<T, T> param_type;
  BoxMuellerDist() : mean_(0), sigma_(1) {}
  BoxMuellerDist(const T mean, const T sigma) : mean_(mean), sigma_(sigma) {}
  const T mean() const { return mean_; }
  const T sigma() const { return sigma_; }
  void mean(const T mean) { mean_ = mean; }
  void sigma(const T sigma) { sigma_ = sigma; }
  const param_type param() const { return std::make_pair(mean_, sigma_); }
  void param(const param_type &p) {
    mean_ = p.first;
    sigma_ = p.second;
  }
  const T min() const { return std::numeric_limits<T>::min(); }
  const T max() const { return std::numeric_limits<T>::max(); }
  const bool operator==(const BoxMuellerDist<T> &rhs) const {
    return mean_ == rhs.mean_ && sigma_ == rhs.sigma_;
  }
  const bool operator!=(const BoxMuellerDist<T> &rhs) const {
    return mean_ != rhs.mean_ || sigma_ != rhs.sigma_;
  }
  template <class Generator>
  T operator() (Generator &g) const {
    static std::uniform_real_distribution<T> uni(0, 1);
    T u1 = uni(g),
      u2 = uni(g),
      r = std::sqrt(-2 * std::log(u1)) * std::cos(2 * M_PI * u2);
    return mean_ + sigma_ * r;
  }
  template <class Generator>
  T operator() (Generator &g, const param_type &p) const {
    static std::uniform_real_distribution<T> uni(0, 1);
    T u1 = uni(g),
      u2 = uni(g),
      r = std::sqrt(-2 * std::log(u1)) * std::cos(2 * M_PI * u2);
    return p.first + p.second * r;
  }
  void reset() const {}
};


template <typename T>
std::ostream &operator<<(std::ostream &out, const BoxMuellerDist<T> &d) {
  out << d.mean() << ", " << d.sigma();
  return out;
}


template <typename T>
std::istream &operator>>(std::istream &in, BoxMuellerDist<T> &d) {
  T mean,
    sigma;
  in >> mean >> sigma;
  d.mean(mean);
  d.sigma(sigma);
  return in;
}

#endif  // __BOXMUELLER_H__
