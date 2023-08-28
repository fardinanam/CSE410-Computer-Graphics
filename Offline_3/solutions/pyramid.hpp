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
  Pyramid(Vector bottomLowerLeft, double width, double height, Color color, double ambient, double diffuse, double reflection, double specular, double shininess) 
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

  // // TODO: review before use
  // Vector normal(const Vector p) {
    


  // }

  Color getColor(Vector p) {
    return Object::getColor();
  }

  Color getColor() {
    return Object::getColor();
  }

  double intersect_t(const Vector p, const Vector d) {
    // call intersect_t on all triangles and base
    // return the minimum of the five

    double min_t = -1;
    for (int i = 0; i < 4; i++) {
      double t = triangles[i].intersect_t(p, d);
      if (t != -1) {
        if (min_t == -1) {
          min_t = t;
        } else {
          min_t = std::min(min_t, t);
        }
      }
    }

    double t = base.intersect_t(p, d);
    if (t != -1) {
      if (min_t == -1) {
        min_t = t;
      } else {
        min_t = std::min(min_t, t);
      }
    }

    return min_t;
  }

  void draw() {
    for (int i = 0; i < 4; i++) {
      triangles[i].draw();
    }

    base.draw();
  }
};
#endif