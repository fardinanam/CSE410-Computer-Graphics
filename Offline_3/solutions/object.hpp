#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "point.hpp"

class Object {
private:
  point color;
  double ambient, diffuse, reflection, specular;
  double shininess;
public:
  Object(point color, double ambient, double diffuse, double reflection, double specular, double shininess) {
    this->color = color;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->reflection = reflection;
    this->specular = specular;
    this->shininess = shininess;
  }
  /**
   * @param p: origin of the ray
   * @param d: direction of the ray (normalized)
   * @return: the distance from p to the intersection point
  */
  virtual double intersect(const point p, const point d) = 0;
  virtual point normal(const point p) = 0;
  virtual void draw() = 0;

  virtual point getColor() {
    return color;
  }

  virtual double getAmbient() {
    return ambient;
  }

  virtual double getDiffuse() {
    return diffuse;
  }

  virtual double getReflection() {
    return reflection;
  }

  virtual double getSpecular() {
    return specular;
  }

  virtual double getShininess() {
    return shininess;
  }
};

#endif // OBJECT_HPP