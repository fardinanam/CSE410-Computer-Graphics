#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"

class Triangle : public Object {
private:
  Vector p1, p2, p3;

public:
  Triangle() {
    p1 = {0, 0, 0};
    p2 = {0, 0, 0};
    p3 = {0, 0, 0};
  }
  
  Triangle(Vector p1, Vector p2, Vector p3, Vector color, double ambient, double diffuse, double reflection, double specular, double shininess) 
    : Object(color, ambient, diffuse, reflection, specular, shininess) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
  }

  // TODO: implement this
  Vector normal(const Vector p) {
    Vector v0 = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
    Vector v1 = {p3.x - p1.x, p3.y - p1.y, p3.z - p1.z};
    Vector n = {v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x};
    double norm = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    n.x /= norm;
    n.y /= norm;
    n.z /= norm;
    return n;
  }

  // TODO: implement this
  double intersect(const Vector p, const Vector d) {
    Vector n = normal(p1);
    double t = (n.x * (p1.x - p.x) + n.y * (p1.y - p.y) + n.z * (p1.z - p.z)) / (n.x * d.x + n.y * d.y + n.z * d.z);
    Vector intersection = {p.x + t * d.x, p.y + t * d.y, p.z + t * d.z};
    Vector v0 = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
    Vector v1 = {p3.x - p1.x, p3.y - p1.y, p3.z - p1.z};
    Vector v2 = {intersection.x - p1.x, intersection.y - p1.y, intersection.z - p1.z};
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
    glBegin(GL_TRIANGLES);
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p2.x, p2.y, p2.z);
    glVertex3f(p3.x, p3.y, p3.z);
    glEnd();
    glPopMatrix();
  }
};
#endif