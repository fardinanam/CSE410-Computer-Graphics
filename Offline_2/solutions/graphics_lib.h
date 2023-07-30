#ifndef GRAPHICS_LIB_H
#define GRAPHICS_LIB_H

#include "matrix.h"
#include "vector3D.h"
#include "bitmap_image.hpp"
#include <stack>
#include <vector>
#include <utility>

Vector3D rodrigues(Vector3D v, Vector3D a, double angle);

class Scene {
private:
    std::stack<Matrix> s;
    Matrix m;
    Vector3D eyePos;
    Vector3D lookDir;
    Vector3D rightDir;
    Vector3D upDir;
    // take a vector of <polygon, color> pair
    std::vector<std::pair<std::vector<Vector3D>, Vector3D> > polygons;

    bitmap_image image;

    double **zBuffer;

    double fovY, aspectRatio, near, far;
    double screenWidth, screenHeight;

    double dx, dy, topY, leftX, zMax;

    const double zBufferLeftLimit = -1.0;
    const double zBufferRightLimit = 1.0;
    const double zBufferBottomLimit = -1.0;
    const double zBufferTopLimit = 1.0;
    const double zFrontLimit = 1.0;
    const double zBackLimit = -1.0;

    std::string inputFilename;
    std::string configFilename;
    const std::string stage1Filename = "stage1.txt";
    const std::string stage2Filename = "stage2.txt";
    const std::string stage3Filename = "stage3.txt";
    const std::string zBufferFilename = "z-buffer.txt";
    const std::string outputFilename = "out.bmp";

    void modelTransformation();
    void transformView();
    void transformProjection();
    void parseConfig();
    void initializeZBuffer();
    void initializeBitmapImg();
    void transformToZBuffer();
    void writeZBufferToFile();
public:
    Scene(std::string inputFilename, std::string configFilename);
    ~Scene();
    
    void push();
    void pop();

    void lookAt(double eyeX, double eyeY, double eyeZ, double lookX, double lookY, double lookZ, 
                double upX, double upY, double upZ);

    void translate(double tx, double ty, double tz);
    void rotate(double angle, double ax, double ay, double az);
    void scale(double sx, double sy, double sz);

    void draw();
};

#endif // GRAPHICS_LIB_H