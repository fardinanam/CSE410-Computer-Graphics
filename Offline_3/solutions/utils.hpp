#ifndef UTILS_HPP
#define UTILS_HPP
#include "vector.hpp"

#define EPSILON 1e-5

struct Light {
  Vector position;
  Vector lookAt;
  double fallOff;
  double cutOffAngle;
};

struct Color {
  double r, g, b;

  Color operator+(const Color &c) const {
    return {r + c.r, g + c.g, b + c.b};
  }

  Color operator*(const double& d) const {
    return {r * d, g * d, b * d};
  }
};

#endif