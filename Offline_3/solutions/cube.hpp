#ifndef CUBE_HPP
#define CUBE_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"

class Cube : public Object {
private:
  point bottomLowerLeft;
  double side;

public:
  Cube(point bottomLowerLeft, double side, point color, double ambient, double diffuse, double reflection, double specular, double shininess) 
    : Object(color, ambient, diffuse, reflection, specular, shininess) {
    this->bottomLowerLeft = bottomLowerLeft;
    this->side = side;
  }

  // TODO: Implement this function properly
  double intersect(point p, point d) {
    double t1 = (bottomLowerLeft.x - p.x) / d.x;
    double t2 = (bottomLowerLeft.x + side - p.x) / d.x;
    double t3 = (bottomLowerLeft.y - p.y) / d.y;
    double t4 = (bottomLowerLeft.y + side - p.y) / d.y;
    double t5 = (bottomLowerLeft.z - p.z) / d.z;
    double t6 = (bottomLowerLeft.z + side - p.z) / d.z;
    double tmin = fmax(fmax(fmin(t1, t2), fmin(t3, t4)), fmin(t5, t6));
    double tmax = fmin(fmin(fmax(t1, t2), fmax(t3, t4)), fmax(t5, t6));
    if (tmax < 0) {
      return -1;
    }
    if (tmin > tmax) {
      return -1;
    }
    if (tmin < 0) {
      return tmax;
    }
    return tmin;
  }

  // implement this function properly
  point normal(point p) {
    point n = {0, 0, 0};
    if (p.x == bottomLowerLeft.x) {
      n.x = -1;
    }
    if (p.x == bottomLowerLeft.x + side) {
      n.x = 1;
    }
    if (p.y == bottomLowerLeft.y) {
      n.y = -1;
    }
    if (p.y == bottomLowerLeft.y + side) {
      n.y = 1;
    }
    if (p.z == bottomLowerLeft.z) {
      n.z = -1;
    }
    if (p.z == bottomLowerLeft.z + side) {
      n.z = 1;
    }
    return n;
  }

  void draw() {
    glColor3f(getColor().x, getColor().y, getColor().z);
    glPushMatrix();
    glTranslatef(bottomLowerLeft.x + side / 2, bottomLowerLeft.y + side / 2, bottomLowerLeft.z + side / 2);
    glutSolidCube(side);
    glPopMatrix();
  }
};
#endif