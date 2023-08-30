#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"
#include "matrix.hpp"

class Triangle : public Object {
private:
  Vector p1, p2, p3;
  
public:
  Triangle() {
    p1 = {0, 0, 0};
    p2 = {0, 0, 0};
    p3 = {0, 0, 0};
  }
  
  Triangle(Vector p1, Vector p2, Vector p3, Color color, double ambient, double diffuse, double reflection, double specular, double shininess) 
    : Object(color, ambient, diffuse, reflection, specular, shininess) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
  }

  Vector normal(const Vector p) {
    Vector v1 = p2 - p1;
    Vector v2 = p3 - p1;

    return v1.cross(v2).normalize();
  }

  Color getColor() {
    return Object::getColor();
  }

  Color getColor(Vector p) {
    return getColor();
  }

  Vector centroid() {
    return (p1 + p2 + p3) / 3;
  }

  double intersect_t(const Vector p, const Vector d) {
    // Barycentric coordinates
    Matrix A = Matrix(3, 3);
    A.set(0, 0, p1.x - p2.x);
    A.set(0, 1, p1.x - p3.x);
    A.set(0, 2, d.x);

    A.set(1, 0, p1.y - p2.y);
    A.set(1, 1, p1.y - p3.y);
    A.set(1, 2, d.y);

    A.set(2, 0, p1.z - p2.z);
    A.set(2, 1, p1.z - p3.z);
    A.set(2, 2, d.z);

    double detA = A.determinant();

    Matrix betaMat = Matrix(3, 3);
    betaMat.set(0, 0, p1.x - p.x);
    betaMat.set(0, 1, p1.x - p3.x);
    betaMat.set(0, 2, d.x);

    betaMat.set(1, 0, p1.y - p.y);
    betaMat.set(1, 1, p1.y - p3.y);
    betaMat.set(1, 2, d.y);

    betaMat.set(2, 0, p1.z - p.z);
    betaMat.set(2, 1, p1.z - p3.z);
    betaMat.set(2, 2, d.z);

    double beta = betaMat.determinant() / detA;

    Matrix gammaMat = Matrix(3, 3);
    gammaMat.set(0, 0, p1.x - p2.x);
    gammaMat.set(0, 1, p1.x - p.x);
    gammaMat.set(0, 2, d.x);

    gammaMat.set(1, 0, p1.y - p2.y);
    gammaMat.set(1, 1, p1.y - p.y);
    gammaMat.set(1, 2, d.y);

    gammaMat.set(2, 0, p1.z - p2.z);
    gammaMat.set(2, 1, p1.z - p.z);
    gammaMat.set(2, 2, d.z);

    double gamma = gammaMat.determinant() / detA;

    Matrix tMat = Matrix(3, 3);
    tMat.set(0, 0, p1.x - p2.x);
    tMat.set(0, 1, p1.x - p3.x);
    tMat.set(0, 2, p1.x - p.x);

    tMat.set(1, 0, p1.y - p2.y);
    tMat.set(1, 1, p1.y - p3.y);
    tMat.set(1, 2, p1.y - p.y);

    tMat.set(2, 0, p1.z - p2.z);
    tMat.set(2, 1, p1.z - p3.z);
    tMat.set(2, 2, p1.z - p.z);

    double t = tMat.determinant() / detA;

    if (beta > -EPSILON && gamma > -EPSILON && beta + gamma < 1 && t > -EPSILON) {
      return t;
    } 

    return -1;
  }

  void draw() {
    glColor3f(getColor().r, getColor().g, getColor().b);
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