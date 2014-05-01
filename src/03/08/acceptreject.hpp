#ifndef __ACCEPTREJECT_HPP__
#define __ACCEPTREJECT_HPP__

#include <cmath>
#include <random>

template <typename T=double>
struct AcceptRejectSinxOver2Dist {
  constexpr const static T minx = 0,
                           maxx = M_PI,
                           miny = 0,
                           maxy = .5;
  typedef T result_type;
  template <class Generator>
  T operator() (Generator &g) {
    std::uniform_real_distribution<T> distx(minx, maxx),
                                      disty(miny, maxy);
    T x,
      y;
    do {
      x = distx(g);
      y = distx(g);
    } while(y > std::sin(x) / 2);
    return x;
  }
};


#endif  // __ACCEPTREJECT_HPP__
