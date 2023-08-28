#ifndef UTILS_HPP
#define UTILS_HPP
#include "vector.hpp"

struct normalLight {
  Vector position;
  double fallOff;
};

struct spotLight {
  Vector position;
  Vector lookAt;
  double fallOff;
  double cutOffAngle;
};

struct Color {
  double r, g, b;
};

#endif