#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"

class Square : public Object {
private:
  point lowerLeft, lowerRight, upperLeft, upperRight;
public:
  Square(point lowerLeft, point lowerRight, point upperLeft, point upperRight
    , point color, double ambient, double diffuse, double reflection, double specular, double shininess) 
    : Object(color, ambient, diffuse, reflection, specular, shininess) {
    this->lowerLeft = lowerLeft;
    this->lowerRight = lowerRight;
    this->upperLeft = upperLeft;
    this->upperRight = upperRight;
  }

  point normal(const point p) {
    point v0 = {lowerRight.x - lowerLeft.x, lowerRight.y - lowerLeft.y, lowerRight.z - lowerLeft.z};
    point v1 = {upperLeft.x - lowerLeft.x, upperLeft.y - lowerLeft.y, upperLeft.z - lowerLeft.z};
    point n = {v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x};
    double norm = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    n.x /= norm;
    n.y /= norm;
    n.z /= norm;
    return n;
  }

  // TODO: Implement this function properly
  double intersect(const point p, const point d) {
    // use barycentric coordinate to find intersection
    point n = normal(lowerLeft);
    double t = (n.x * (lowerLeft.x - p.x) + n.y * (lowerLeft.y - p.y) + n.z * (lowerLeft.z - p.z)) / (n.x * d.x + n.y * d.y + n.z * d.z);
    point intersection = {p.x + t * d.x, p.y + t * d.y, p.z + t * d.z};
    point v0 = {lowerRight.x - lowerLeft.x, lowerRight.y - lowerLeft.y, lowerRight.z - lowerLeft.z};
    point v1 = {upperLeft.x - lowerLeft.x, upperLeft.y - lowerLeft.y, upperLeft.z - lowerLeft.z};
    point v2 = {intersection.x - lowerLeft.x, intersection.y - lowerLeft.y, intersection.z - lowerLeft.z};
    double dot00 = v0.x * v0.x + v0.y * v0.y + v0.z * v0.z;
    double dot01 = v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
    double dot02 = v0.x * v2.x + v0.y * v2.y + v0.z * v2.z;
    double dot11 = v1.x * v1.x + v1.y * v1.y + v1.z * v1.z;
    double dot12 = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    if (u >= 0 && u <= 1 && v >= 0 && v <= 1) {
      return t;
    }
    return -1;
  }

  void draw() {
    glColor3f(getColor().x, getColor().y, getColor().z);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(lowerLeft.x, lowerLeft.y, lowerLeft.z);
    glVertex3f(lowerRight.x, lowerRight.y, lowerRight.z);
    glVertex3f(upperRight.x, upperRight.y, upperRight.z);
    glVertex3f(upperLeft.x, upperLeft.y, upperLeft.z);
    glEnd();
    glPopMatrix();
  }
};

#endif