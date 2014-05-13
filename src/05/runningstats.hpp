#ifndef __RUNNINGSTATS_HPP__
#define __RUNNINGSTATS_HPP__

#include <cmath>
#include <tuple>


template <typename T=double>
class RunningStats {
 private:
  size_t k_;
  T M_,
    S_;
 public:
  typedef std::tuple<T, T> mean_and_sem_type;
  RunningStats() : k_(0), M_(0), S_(0) {}
  void Clear() { k_ = 0; }
  void Push(const T &x) {
    ++k_;
    if (k_ == 1) {  // first value
      M_ = x;
      S_ = 0;
    } else {
      const T old_M = M_;
      M_ = M_ + (x - M_) / k_;
      S_ = S_ + (x - old_M) * (x - M_);
    }
  }
  const size_t n() const { return k_; }
  const T Mean() const {
    if (k_ > 0) {
      return M_;
    }
    return 0;
  }
  const T Variance() const {
    if (k_ > 1) {
      return S_ / (k_ - 1);
    }
    return 0;
  }
  const T StdDev() const {
    return std::sqrt(Variance());
  }
  const T SEM() const {
    if (k_ > 0) {
      return StdDev() / std::sqrt(k_);
    }
    return 0;
  }
  const mean_and_sem_type MeanAndSEM() const {
    return std::make_tuple(Mean(), SEM());
  }
};

#endif  // __RUNNINGSTATS_HPP__
