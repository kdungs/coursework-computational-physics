#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <sstream>
#include <iomanip>

#include "Eigen/Eigen"

typedef Eigen::Vector2d Vec2d;

class System {
 private:
  bool _initialised;
  const size_t _N;
  double _L;
  std::vector<Vec2d> _r,
                     _v;

 public:
  System(const size_t N) : _initialised(false), _N(N), _r(N), _v(N) {}
  
  template <typename URNG>
  void initialise(
    URNG &rng,
    const double scale_v=1,
    const double L=4
  ) {
    _L = L;
    std::uniform_real_distribution<> dist_r(0, _L);
    std::generate(std::begin(_r), std::end(_r),
      [&] () {
        return Vec2d{dist_r(rng), dist_r(rng)};
      });
    std::uniform_real_distribution<> dist_v(-scale_v, scale_v);
    std::generate(std::begin(_v), std::end(_v),
      [&] () {
        return Vec2d{dist_v(rng), dist_v(rng)};
      });
    const Vec2d mean_v = std::accumulate(
      std::begin(_v),
      std::end(_v),
      Vec2d{0.0, 0.0},
      [&] (const Vec2d &mean, const Vec2d &v) {
        return mean + v / _N;
      });
    std::transform(std::begin(_v), std::end(_v), std::begin(_v),
      [&] (const Vec2d &v) {
        return v - mean_v;
      });
    
    _initialised = true;
  }

  double size() const{
    return _L;
  }

  void resize(const double L){
    _L = L;
  }

  void respeed(const double dv){
    for(Vec2d& v : _v){
      v *= dv;
    }
  }

  double Ekin() const{
    double E = 0;
    for(const Vec2d& v : _v){
      E += 1./2 * (v.x() * v.x() + v.y() * v.y());
    }
    return E;
  }

  double Epot(double (*V)(const double r)) const{
    std::vector<Vec2d> offsets({
      Vec2d{0, 0},
      Vec2d{-_L, -_L},
      Vec2d{0, -_L},
      Vec2d{_L, -_L},
      Vec2d{_L, 0},
      Vec2d{_L, _L},
      Vec2d{0, _L},
      Vec2d{-_L, _L},
      Vec2d{-_L, 0},
    });
    double E = 0;
    for(const Vec2d& r1 : _r){
      for(const Vec2d& offset : offsets){
        for(const Vec2d& r2 : _r){
          Vec2d dr = r1 - (r2 + offset);
          if(dr != Vec2d{0, 0}){
            double dist = std::sqrt(dr.x() * dr.x() + dr.y()*dr.y());
            E += V(dist);
          }
        }
      }
    }
    return E;
  }

  void print() const {
    std::cout << "r: ";
    std::copy(std::begin(_r), std::end(_r),
              std::ostream_iterator<Vec2d>(std::cout, ",\n"));
    std::cout << "v: ";
    std::copy(std::begin(_v), std::end(_v),
              std::ostream_iterator<Vec2d>(std::cout, ",\n"));
  }

  void printFirst() const {
    std::cout << _r[0].x() << std::endl;
  }

  void save(const std::string& filename) const {
    std::ofstream ofs(filename);
    for(size_t i=0; i < _N; i++){
      ofs << _r[i].x() << "\t" << _r[i].y() << "\n";
    }
    ofs.close();
  }

  void timeStep(
    const double& dt,
    double (*dV)(const double r),
    const double& cutoff
  ) {
    std::vector<Vec2d> r_before(_r),
                       v_before(_v),
                       a_vec(_N);

    std::vector<Vec2d> offsets({
      Vec2d{0, 0},
      Vec2d{-_L, -_L},
      Vec2d{0, -_L},
      Vec2d{_L, -_L},
      Vec2d{_L, 0},
      Vec2d{_L, _L},
      Vec2d{0, _L},
      Vec2d{-_L, _L},
      Vec2d{-_L, 0},
    });

    // 1. Velocity-Verlet: accumulate r(t + dt)
    for(size_t i=0; i<_N; i++){
      Vec2d a{0, 0};
      for(Vec2d offset : offsets){
        // accumulate the acceleration caused by the field of
        // all surrounding particles
        for(size_t j=0; j<_N; j++){ // replace with std::accumulate later on...
          Vec2d dr = r_before[i] - r_before[j] + offset;
          double distance = std::sqrt(dr.x()*dr.x() + dr.y()*dr.y());
          if(i != j && distance < cutoff){
            a += dr / distance * dV(distance);
          } 
        }
      }
      a_vec[i] = a;
      _r[i] = r_before[i] + dt * v_before[i] + 1./2 * a * dt*dt;
      _r[i].x() = fmod(_r[i].x() + _L, _L);
      _r[i].y() = fmod(_r[i].y() + _L, _L);
    }
    // 2. Velocity-Verlet: accumulate v(t + dt)
    for(size_t i=0; i<_N; i++){
      Vec2d a_new{0, 0};
      for(Vec2d offset : offsets){
        // accumulate the acceleration caused by the field of
        // all surrounding particles
        for(size_t j=0; j<_N; j++){ // replace with std::accumulate later on...
          Vec2d dr = _r[i] - _r[j] + offset;
          double distance = std::sqrt(dr.x()*dr.x() + dr.y()*dr.y());
          if(i != j && distance < cutoff){
            a_new += dr / distance * dV(distance);
          } 
        }
      }
      _v[i] = v_before[i] + 1./2 * a_vec[i] * dt + 1./2 * a_new * dt;
    }
  }
};

double V(const double r){
  return 4. * (pow(1./r, 12) - pow(1./r, 6));
}

double dV(const double r){
  return - 24. * (pow(1./r, 7) - 2 * pow(1./r, 13));
};

int main(int argc, char *argv[]) {
  std::random_device rd;
  std::mt19937 rng(rd());

  std::ofstream energies("energies.txt");

  System s(24);
  s.initialise(rng, 1, 5);
  const double step = 1e-8;
  for(size_t frame=0; frame<3600; frame++){
    std::cout << "\r" << frame << std::flush;
    for(double time=0; time<1e-6; time+=step){
      s.timeStep(step, dV, 3.0);
    }
    if(frame < 1800){
      s.resize(5. - 3. * (1./1800 * frame));
    }
    // s.respeed(1.001);
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << frame;
    s.save("data/" + ss.str() + ".txt");
    energies << s.Ekin()
      << "\t" << s.Epot(V)
      << "\t" << s.size()
      << "\n";
  }
  energies.close();
  std::cout << std::endl;
}
