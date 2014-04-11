/** @file point.h
 * Class definition for a point in the two dimensional plane.
 */
#ifndef __POINT_H__
#define __POINT_H__

/** A point in the two dimensional plane.
 */
class Point {
 private:
  int x_,
      y_;
 public:
  Point() = default;
  Point(int x, int y) : x_(x), y_(y) {}
  Point operator+(const Point &other) const {
    return {x_ + other.x_, y_ + other.y_};
  }
  bool operator==(const Point &other) const {
    return x_ == other.x_ && y_ == other.y_;
  }
  void Print() const {
    std::cout << x_ << " " << y_ << std::endl;
  }
  double Squared() const {
    return x_ * x_ + y_ * y_;
  }
};

#endif /* __POINT_H__ */
