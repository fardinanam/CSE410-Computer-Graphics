#ifndef CHECKERBOARD_HPP
#define CHECKERBOARD_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"
#include "quadrilateral.hpp"

class Checkerboard : public Object {
private:
  double width;
  double startX, startZ;
  int n = 50;

  Color getColor(int x, int z) {
    if ((x + z) % (2 * (int)width) == 0) {
      return {0, 0, 0};
    } else {
      return {1, 1, 1};
    }
  }
public:
  Checkerboard() {
    width = 0;
  }

  Checkerboard(double width, double ambient, double diffuse, double reflection, double shininess) 
    : Object({0, 0, 0}, ambient, diffuse, reflection, 0, shininess) {
    this->width = width;
    startX = -n * width / 2;
    startZ = n * width / 2;
  }

  double intersect_t(const Vector p, const Vector d) {
    Vector lowerLeft(startX, 0, startZ);
    Vector lowerRight(startX + n * width, 0, startZ);
    Vector upperLeft(startX, 0, startZ - n * width);
    Vector upperRight(startX + n * width, 0, startZ - n * width);

    Quadrilateral quad = Quadrilateral(lowerLeft, lowerRight, upperRight, upperLeft, 
      getColor(), getAmbient(), getDiffuse(), getReflection(), getSpecular(), getShininess());

    return quad.intersect_t(p, d);
  }

  Vector normal(const Vector p) {
    return {0, 1, 0};
  }

  Color getColor() {
    return Object::getColor();
  }

  Color getColor(Vector p) {
    // find the closest point on the checkerboard to p
    // calculate the cell index of that point
    // if the cell index is even, return black
    // if the cell index is odd, return white

    int posX;
    int posZ;
    posX = (int)floor(p.x);
    posZ = (int)floor(p.z);

    if (p.x > 0) {
      posX = posX - (posX % (int)width);
    } else {
      posX = posX - (posX % (int)width)  - (int)width; 
    }

    if (p.z > 0) {
      posZ = posZ - (posZ % (int)width);
    } else {
      posZ = posZ - (posZ % (int)width)  - (int)width;
    }

    return getColor(posX, posZ);
  }
  
  void draw() {}

  void draw(Vector position) {
    // draw a 200 x 200 checkerboard around the position
    // the position is the center of the checkerboard
    // the checkerboard is on the xz plane
    // the checkerboard is made of width X width squares
    // the color of the squares alternate between black and white
    int posX = (int)position.x;
    int posZ = (int)position.z;

    posX = posX - (posX % (int)width);
    posZ = posZ - (posZ % (int)width);

    startX = posX - n * (int)width / 2;
    startZ = posZ + n * (int)width / 2;

    for (int i = 0; i < n; i++) {
      int x = startX + i * (int)width;
      for (int j = 0; j < n; j++) {
        int z = startZ - j * (int)width;
        Color color = getColor(x, z);
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_QUADS);
        glVertex3f(x, 0, z);
        glVertex3f(x + width, 0, z);
        glVertex3f(x + width, 0, z + width);
        glVertex3f(x, 0, z + width);
        glEnd();
      }
    }
  }
};

#endif