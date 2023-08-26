#ifndef POINT_HPP
#define POINT_HPP

struct point {
  double x, y, z;
};

point add(const point x, const point y) {
  point result;
  result.x = x.x + y.x;
  result.y = x.y + y.y;
  result.z = x.z + y.z;
  return result;
}

point subtract(const point x, const point y) {
  point result;
  result.x = x.x - y.x;
  result.y = x.y - y.y;
  result.z = x.z - y.z;
  return result;
}

#endif