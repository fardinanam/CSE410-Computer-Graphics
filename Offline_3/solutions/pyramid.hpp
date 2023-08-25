#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"
#include "triangle.hpp"

class Pyramid : public Object {
private:
  point bottomLowerLeft;
  Triangle *triangles[4];
  double width, height;

public:
  Pyramid(point bottomLowerLeft, double width, double height, point color, double ambient, double diffuse, double reflection, double specular, double shininess) 
    : Object(color, ambient, diffuse, reflection, specular, shininess) {
    this->bottomLowerLeft = bottomLowerLeft;
    this->width = width;
    this->height = height;

    point bottomLowerRight = {bottomLowerLeft.x + width, bottomLowerLeft.y, bottomLowerLeft.z};
    point bottomUpper = {bottomLowerLeft.x + width / 2, bottomLowerLeft.y, bottomLowerLeft.z - sqrt(3) * width / 2};
    point centroid = {bottomLowerLeft.x + width / 2, bottomLowerLeft.y + height, bottomLowerLeft.z - width / 2};
    point top = {centroid.x, centroid.y + height, centroid.z};

    triangles[0] = new Triangle(bottomLowerLeft, bottomLowerRight, bottomUpper, color, ambient, diffuse, reflection, specular, shininess);
    triangles[1] = new Triangle(bottomLowerLeft, bottomUpper, top, color, ambient, diffuse, reflection, specular, shininess);
    triangles[2] = new Triangle(bottomLowerLeft, top, bottomLowerRight, color, ambient, diffuse, reflection, specular, shininess);
    triangles[3] = new Triangle(bottomLowerRight, top, bottomUpper, color, ambient, diffuse, reflection, specular, shininess);
  }

  point normal(const point p) {
    point v0 = {bottomLowerLeft.x + width / 2, bottomLowerLeft.y + height, bottomLowerLeft.z + width / 2};
    point v1 = {bottomLowerLeft.x + width / 2, bottomLowerLeft.y + height, bottomLowerLeft.z - width / 2};
    point v2 = {bottomLowerLeft.x - width / 2, bottomLowerLeft.y + height, bottomLowerLeft.z - width / 2};
    point v3 = {bottomLowerLeft.x - width / 2, bottomLowerLeft.y + height, bottomLowerLeft.z + width / 2};
    point n = {v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x};
    double norm = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    n.x /= norm;
    n.y /= norm;
    n.z /= norm;
    return n;
  }

  // TODO: review before use
  double intersect(const point p, const point d) {
    double t = triangles[0]->intersect(p, d);
    for (int i = 1; i < 4; i++) {
      double t2 = triangles[i]->intersect(p, d);
      if (t2 != -1 && t2 < t) {
        t = t2;
      }
    }

    return t;
  }

  void draw() {
    for (int i = 0; i < 4; i++) {
      triangles[i]->draw();
    }
  }

  ~Pyramid() {
    for (int i = 0; i < 4; i++) {
      delete triangles[i];
    }
  }
};
#endif