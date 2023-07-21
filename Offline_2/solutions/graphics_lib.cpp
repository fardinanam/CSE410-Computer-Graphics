#include "graphics_lib.h"
#include "matrix.h"
#include "vector3D.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>

#define M_PI 3.14159265358979323846

/**
 * Creates a new scene with the 4x4 identity matrix as the
 * initial transformation. Eye is set to (0, 0, 1), look is set to (0, 0, 0)
 * and up is set to (0, 1, 0)
*/
Scene::Scene() {
    m = Matrix(4, 4);
    rightDir = Vector3D(1, 0, 0);
    lookDir = Vector3D(0, 0, -1);
    upDir = Vector3D(0, 1, 0);
    
    for (int i = 0; i < 4; i++) {
        m.set(i, i, 1);
    }
}

Scene::~Scene() {}

/**
 * Calculates the camera axes from eye position, look at 
 * position and up direction and save the new points in 
 * the stage2 file
 * 
 * @param eyeX The x component of the eye position.
 * @param eyeY The y component of the eye position.
 * @param eyeZ The z component of the eye position.
 * @param lookX The x component of the look at position.
 * @param lookY The y component of the look at position.
 * @param lookZ The z component of the look at position.
 * @param upX The x component of the up direction.
 * @param upY The y component of the up direction.
 * @param upZ The z component of the up direction.
*/
void Scene::lookAt(double eyeX, double eyeY, double eyeZ, double lookX, double lookY, double lookZ, 
                double upX, double upY, double upZ) {
    Vector3D lookAt(lookX, lookY, lookZ);
    Vector3D up(upX, upY, upZ);
    eyePos = Vector3D(eyeX, eyeY, eyeZ);

    // calculate the look direction
    lookDir = (lookAt - eyePos).normalize();

    // calculate the right direction
    rightDir = lookDir.cross(up).normalize();

    // calculate the up direction
    upDir = rightDir.cross(lookDir).normalize();
}

/**
 * Pushes the current transformation matrix onto the stack.
*/
void Scene::push() {
    s.push(m);
}

/**
 * Pops the top matrix from the stack and sets the current
 * transformation matrix to the popped matrix.
*/
void Scene::pop() {
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
void Scene::translate(double tx, double ty, double tz) {
    // create a 4x4 translation matrix
    Matrix transMat = Matrix(4, 4);
    transMat.set(0, 3, tx);
    transMat.set(1, 3, ty);
    transMat.set(2, 3, tz);

    for (int i = 0; i < 4; i++) {
        transMat.set(i, i, 1);
    }

    // multiply the current matrix by the translation matrix
    m = m * transMat;

    // std::cout << m << std::endl;
}

/**
 * Multiplies the current transformation matrix by a scaling matrix
 * with the given scaling vector.
 * 
 * @param sx The x component of the scaling vector.
 * @param sy The y component of the scaling vector.
 * @param sz The z component of the scaling vector.
*/
void Scene::scale(double sx, double sy, double sz) {
    // create a 4x4 scaling matrix
    Matrix scaleMat = Matrix(4, 4);
    scaleMat.set(0, 0, sx);
    scaleMat.set(1, 1, sy);
    scaleMat.set(2, 2, sz);
    scaleMat.set(3, 3, 1);

    // multiply the current matrix by the scaling matrix
    m = m * scaleMat;
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
void Scene::rotate(double angle, double ax, double ay, double az) {
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

    m = m * rotMat;
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
 * Transforms the view of the scene by multiplying the current view transformation matrix
*/
void Scene::transformView() {
    // create the view matrix
    Matrix T = Matrix(4, 4);
    T.set(0, 3, -eyePos.getX());
    T.set(1, 3, -eyePos.getY());
    T.set(2, 3, -eyePos.getZ());

    for (int i = 0; i < 4; i++) {
        T.set(i, i, 1);
    }

    Matrix R = Matrix(4, 4);
    R.set(0, 0, rightDir.getX());
    R.set(0, 1, rightDir.getY());
    R.set(0, 2, rightDir.getZ());

    R.set(1, 0, upDir.getX());
    R.set(1, 1, upDir.getY());
    R.set(1, 2, upDir.getZ());

    R.set(2, 0, -lookDir.getX());
    R.set(2, 1, -lookDir.getY());
    R.set(2, 2, -lookDir.getZ());

    R.set(3, 3, 1);

    Matrix V = R * T;

    // std::cout << "R:\n" << R << std::endl;
    // std::cout << "T:\n" << T << std::endl;
    // std::cout << "V:\n" << V << std::endl;

    // transform all the points of stage 1 with the new 
    // view matrix and save them in stage 2
    std::ifstream stage1File(stage1Filename);
    std::ofstream stage2File(stage2Filename);
    std::string line;
    
    while (std::getline(stage1File, line)) {
        if (line == "") {
            stage2File << std::endl;
            continue;
        }

        std::istringstream iss(line);
        double x, y, z;
        iss >> x >> y >> z;

        Vector3D v = Vector3D(x, y, z);
        v = Vector3D(V * v.toHomogeneousMatrix());

        stage2File << std::fixed << std::setprecision(7) << v.getX() << " " << v.getY() << " " << v.getZ() << std::endl;
    }
}

/**
 * Reads the scene from a file and executes the commands.
 * 
 * @param filename The name of the file containing the scene.
*/
void Scene::drawScene(std::string filename) {
    // read the file line by line
    std::ifstream sceneFile(filename);
    std::ofstream stage1File(stage1Filename);
    std::string line;
    double eyeX, eyeY, eyeZ;
    double lookX, lookY, lookZ;
    double upX, upY, upZ;

    int lineno = 0;
    while (std::getline(sceneFile, line)) {
        lineno++;

        if (lineno == 1) {
            std::istringstream iss(line);
            iss >> eyeX >> eyeY >> eyeZ;
        } else if (lineno == 2) {
            std::istringstream iss(line); 
            iss >> lookX >> lookY >> lookZ;
        } else if (lineno == 3) {
            std::istringstream iss(line);
            iss >> upX >> upY >> upZ;

            lookAt(eyeX, eyeY, eyeZ,
                    lookX, lookY, lookZ,
                    upX, upY, upZ);
            // std::cout << "eyePos:" <<  eyePos << std::endl;
            // std::cout << "lookDir" << lookDir << std::endl;
            // std::cout << "rightDir" << rightDir << std::endl;
            // std::cout << "upDir" << upDir << std::endl;
        } else if (line == "triangle") {
            for (int i = 0; i < 3 && std::getline(sceneFile, line); i++) {
                // read the coordinates of the triangle
                std::istringstream iss(line);
                double x, y, z;
                iss >> x >> y >> z;

                // transform the coordinates
                Vector3D v = Vector3D(x, y, z);
                v = Vector3D(m * v.toHomogeneousMatrix());

                // draw the triangle (print for now)
                stage1File << std::fixed << std::setprecision(7) << v.getX() << " " << v.getY() << " " << v.getZ() << std::endl;
            }

            stage1File << std::endl;
        } else if (line == "translate") {
            std::getline(sceneFile, line);
            std::istringstream iss(line);

            double tx, ty, tz;
            iss >> tx >> ty >> tz;

            translate(tx, ty, tz);
        } else if (line == "scale") {
            std::getline(sceneFile, line);
            std::istringstream iss(line);

            double sx, sy, sz;
            iss >> sx >> sy >> sz;

            scale(sx, sy, sz);
        } else if (line == "rotate") {
            std::getline(sceneFile, line);
            std::istringstream iss(line);

            double angle, ax, ay, az;
            iss >> angle >> ax >> ay >> az;

            rotate(angle, ax, ay, az);
        } else if (line == "push") {
            push();
        } else if (line == "pop") {
            pop();
        } else if (line == "end") {
            break;
        }
    }

    stage1File.close();
    sceneFile.close();

    transformView();
}