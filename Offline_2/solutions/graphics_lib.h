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
    Vector3D eyePos;
    Vector3D lookDir;
    Vector3D rightDir;
    Vector3D upDir;

    const std::string stage1Filename = "stage1.txt";
    const std::string stage2Filename = "stage2.txt";
    const std::string stage3Filename = "stage3.txt"; 
public:
    Scene();
    ~Scene();
    
    void push();
    void pop();

    void lookAt(double eyeX, double eyeY, double eyeZ, double lookX, double lookY, double lookZ, 
                double upX, double upY, double upZ);

    void translate(double tx, double ty, double tz);
    void rotate(double angle, double ax, double ay, double az);
    void scale(double sx, double sy, double sz);

    void transformView();
    void drawScene(std::string filename);
};

#endif // GRAPHICS_LIB_H