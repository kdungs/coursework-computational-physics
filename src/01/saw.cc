#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

class Point {
 private:
  int x_,
      y_;
 public:
  Point() = default;
  Point(int x, int y) : x_(x), y_(y) {}
  const int x() const { return x_; }
  const int y() const { return y_; }
  Point operator+(const Point &other) const {
    return {x_ + other.x_, y_ + other.y_};
  }
  bool operator==(const Point &other) const {
    return x_ == other.x_ && y_ == other.y_;
  }
};

void PrintPoint(const Point &p) {
  std::cout <<  p.x() << " " << p.y() << std::endl;
}


bool SelfAvoidingWalk(
    const size_t step,
    const size_t steps,
    std::vector<Point> &walk,
    std::mt19937 &random_engine
) {
  if (step == steps) {
    return true;
  }
  std::vector<Point> directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  std::shuffle(std::begin(directions), std::end(directions), random_engine);
  for (const Point &direction : directions) {
    auto new_point = walk[step - 1] + direction;
    if (std::find(std::begin(walk), std::end(walk), new_point)
        != std::end(walk)) {
      continue;
    }
    walk[step] = walk[step - 1] + direction;
    bool success = SelfAvoidingWalk(step + 1, steps, walk, random_engine);
    if (success) {  // We're done here and can go home
      return true;
    }  // otherwise we have to try the next direction
  }
  return false;  // if no direction succeeded, we return home shamefully
}

int main(int argc, char *argv[]) {
  // Set up random number generator
  std::random_device rd;
  std::mt19937 rng(rd());
  // Run random walk
  std::vector<Point> walk(100);
  if (SelfAvoidingWalk(1, 100, walk, rng)) {
    std::for_each(std::begin(walk), std::end(walk), [](const Point &p) {
      PrintPoint(p);
    });
  }
}
