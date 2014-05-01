#ifndef __TRANSFORMATION_HPP__
#define __TRANSFORMATION_HPP__

#include <cmath>
#include <random>


template <typename T=double>
struct Transformation3xSquaredDist {
  typedef T result_type;
  template <class Generator>
  T operator() (Generator &g) {
    std::uniform_real_distribution<T> dist(0, 1);
    return std::pow(dist(g), 1. / 3);
  }
};

#endif  // __TRANSFORMATION_HPP__
