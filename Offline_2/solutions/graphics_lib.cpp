#include "graphics_lib.h"
#include "vector3D.h"
#include <iostream>
#include <cmath>
#include <fstream>

#define M_PI 3.14159265358979323846

/**
 * Creates a new scene with the 4x4 identity matrix as the
 * initial transformation.
*/
void initScene() {
    s = std::stack<Matrix>();
    m = Matrix();
}

/**
 * Pushes the current transformation matrix onto the stack.
*/
void push() {
    s.push(m);
}

/**
 * Pops the top matrix from the stack and sets the current
 * transformation matrix to the popped matrix.
*/
void pop() {
    m = s.top();
    s.pop();
}

/**
 * Multiplies the current transformation matrix by a translation
 * matrix with the given translation vector.
 * 
 * @param tx The x component of the translation vector.
 * @param ty The y component of the translation vector.
 * @param tz The z component of the translation vector.
*/
void translate(double tx, double ty, double tz) {
    // create a 4x4 translation matrix
    Matrix transMat = Matrix(4, 4);
    transMat.set(0, 3, tx);
    transMat.set(1, 3, ty);
    transMat.set(2, 3, tz);

    for (int i = 0; i < 4; i++) {
        transMat.set(i, i, 1);
    }

    // multiply the current matrix by the translation matrix
    m = transMat * m;
}

/**
 * Multiplies the current transformation matrix by a scaling matrix
 * with the given scaling vector.
 * 
 * @param sx The x component of the scaling vector.
 * @param sy The y component of the scaling vector.
 * @param sz The z component of the scaling vector.
*/
void scale(double sx, double sy, double sz) {
    // create a 4x4 scaling matrix
    Matrix scaleMat = Matrix(4, 4);
    scaleMat.set(0, 0, sx);
    scaleMat.set(1, 1, sy);
    scaleMat.set(2, 2, sz);
    scaleMat.set(3, 3, 1);

    // multiply the current matrix by the scaling matrix
    m = scaleMat * m;
}

/**
 * Multiplies the current transformation matrix by a rotation matrix
 * with the given rotation vector.
 * 
 * @param angle The angle of rotation in degrees.
 * @param ax The x component of the rotation vector.
 * @param ay The y component of the rotation vector.
 * @param az The z component of the rotation vector.
*/
void rotate(double angle, double ax, double ay, double az) {
    // create a 4x4 rotation matrix
    Vector3D a = Vector3D(ax, ay, az).normalize();
    Vector3D i = Vector3D(1, 0, 0);
    Vector3D j = Vector3D(0, 1, 0);
    Vector3D k = Vector3D(0, 0, 1);

    Vector3D c1 = rodrigues(i, a, angle);
    Vector3D c2 = rodrigues(j, a, angle);
    Vector3D c3 = rodrigues(k, a, angle);

    Matrix rotMat = Matrix(4, 4);
    rotMat.set(0, 0, c1.getX());
    rotMat.set(0, 1, c2.getX());
    rotMat.set(0, 2, c1.getX());

    rotMat.set(1, 0, c1.getY());
    rotMat.set(1, 1, c2.getY());
    rotMat.set(1, 2, c3.getY());

    rotMat.set(2, 0, c1.getZ());
    rotMat.set(2, 1, c2.getZ());
    rotMat.set(2, 2, c3.getZ());

    rotMat.set(3, 3, 1);
}

/**
 * Returns the Rodrigues rotation of a vector about an axis.
*/
Vector3D rodrigues(Vector3D v, Vector3D a, double angle) {
    double theta = angle * M_PI / 180.0;
    double c = cos(theta);
    double s = sin(theta);

    Vector3D result = v * c;
    result = result + (s * a.cross(v));
    result = result + ((1 - c) * a.dot(v) * a);

    return result;
}

/**
 * Reads the scene from a file and executes the commands.
 * 
 * @param filename The name of the file containing the scene.
*/
void scene(std::string filename) {
    // read the file line by line
    std::ifstream sceneFile(filename);
    std::string line;

    while (std::getline(sceneFile, line)) {
        /* code */
    }
    
}