#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"
#include "triangle.hpp"
#include "quadrilateral.hpp"

class Pyramid : public Object {
private:
  Vector bottomLowerLeft;
  Triangle triangles[4];
  Quadrilateral base;
  double width, height;

public:
  Pyramid(Vector bottomLowerLeft, double width, double height, Vector color, double ambient, double diffuse, double reflection, double specular, double shininess) 
    : Object(color, ambient, diffuse, reflection, specular, shininess) {
    this->bottomLowerLeft = bottomLowerLeft;
    this->width = width;
    this->height = height;

    Vector bottomLowerRight = {bottomLowerLeft.x + width, bottomLowerLeft.y, bottomLowerLeft.z};
    Vector bottomUpperLeft = {bottomLowerLeft.x, bottomLowerLeft.y, bottomLowerLeft.z - width};
    Vector bottomUpperRight = {bottomLowerLeft.x + width, bottomLowerLeft.y, bottomLowerLeft.z - width};
    Vector top = {bottomLowerLeft.x + width / 2, bottomLowerLeft.y + height, bottomLowerLeft.z - width / 2};

    triangles[0] = Triangle(bottomLowerLeft, bottomLowerRight, top, color, ambient, diffuse, reflection, specular, shininess);
    triangles[1] = Triangle(bottomLowerLeft, bottomUpperLeft, top, color, ambient, diffuse, reflection, specular, shininess);
    triangles[2] = Triangle(bottomUpperLeft, bottomUpperRight, top, color, ambient, diffuse, reflection, specular, shininess);
    triangles[3] = Triangle(bottomLowerRight, bottomUpperRight, top, color, ambient, diffuse, reflection, specular, shininess);
    base = Quadrilateral(bottomLowerLeft, bottomLowerRight, bottomUpperRight, bottomUpperLeft, color, ambient, diffuse, reflection, specular, shininess);
  }

  // TODO: review before use
  Vector normal(const Vector p) {
    Vector v0 = {bottomLowerLeft.x + width / 2, bottomLowerLeft.y + height, bottomLowerLeft.z + width / 2};
    Vector v1 = {bottomLowerLeft.x + width / 2, bottomLowerLeft.y + height, bottomLowerLeft.z - width / 2};
    Vector v2 = {bottomLowerLeft.x - width / 2, bottomLowerLeft.y + height, bottomLowerLeft.z - width / 2};
    Vector v3 = {bottomLowerLeft.x - width / 2, bottomLowerLeft.y + height, bottomLowerLeft.z + width / 2};
    Vector n = {v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x};
    double norm = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    n.x /= norm;
    n.y /= norm;
    n.z /= norm;
    return n;
  }

  // TODO: review before use
  double intersect(const Vector p, const Vector d) {
    double t = triangles[0].intersect(p, d);
    for (int i = 1; i < 4; i++) {
      double t2 = triangles[i].intersect(p, d);
      if (t2 != -1 && t2 < t) {
        t = t2;
      }
    }

    return t;
  }

  void draw() {
    for (int i = 0; i < 4; i++) {
      triangles[i].draw();
    }

    base.draw();
  }
};
#endif