#ifndef GRAPHICS_LIB_H
#define GRAPHICS_LIB_H

#include "matrix.h"
#include "vector3D.h"
#include <stack>

std::stack<Matrix> s;
Matrix m;

void initScene();
void push();
void pop();

void translate(double tx, double ty, double tz);
void rotate(double angle, double ax, double ay, double az);
void scale(double sx, double sy, double sz);
Vector3D rodrigues(Vector3D v, Vector3D a, double angle);

void scene(std::string filename);

#endif // GRAPHICS_LIB_H