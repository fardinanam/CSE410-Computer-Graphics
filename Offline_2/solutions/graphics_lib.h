#ifndef GRAPHICS_LIB_H
#define GRAPHICS_LIB_H

#include "matrix.h"
#include "vector3D.h"
#include <stack>

Vector3D rodrigues(Vector3D v, Vector3D a, double angle);

class Scene {
private:
    std::stack<Matrix> s;
    Matrix m;
public:
    Scene();
    ~Scene();
    
    void push();
    void pop();

    void translate(double tx, double ty, double tz);
    void rotate(double angle, double ax, double ay, double az);
    void scale(double sx, double sy, double sz);

    void drawScene(std::string filename);
};

#endif // GRAPHICS_LIB_H