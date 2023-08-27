#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"

class Sphere : public Object {
private:
  Vector center;
  double radius;

public:
  Sphere(Vector center, double radius, Vector color, double ambient, double diffuse, double reflection, double specular, double shininess) 
    : Object(color, ambient, diffuse, reflection, specular, shininess) {
    this->center = center;
    this->radius = radius;
  }

  // TODO: Implement this function properly
  double intersect(const Vector p, const Vector d) {
    double a = d.x * d.x + d.y * d.y + d.z * d.z;
    double b = 2 * (d.x * (p.x - center.x) + d.y * (p.y - center.y) + d.z * (p.z - center.z));
    double c = (p.x - center.x) * (p.x - center.x) + (p.y - center.y) * (p.y - center.y) + (p.z - center.z) * (p.z - center.z) - radius * radius;
    double delta = b * b - 4 * a * c;
    if (delta < 0) {
      return -1;
    }
    double t1 = (-b + sqrt(delta)) / (2 * a);
    double t2 = (-b - sqrt(delta)) / (2 * a);
    if (t1 < 0 && t2 < 0) {
      return -1;
    }
    if (t1 < 0) {
      return t2;
    }
    if (t2 < 0) {
      return t1;
    }
    return t1 < t2 ? t1 : t2;
  }

  Vector normal(const Vector p) {
    Vector n = {p.x - center.x, p.y - center.y, p.z - center.z};
    double norm = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    n.x /= norm;
    n.y /= norm;
    n.z /= norm;
    return n;
  }

  void draw() {
    glColor3f(getColor().x, getColor().y, getColor().z);
    glPushMatrix();
    glTranslatef(center.x, center.y, center.z);
    glutSolidSphere(radius, 100, 100);
    glPopMatrix();
  }
};

#endif