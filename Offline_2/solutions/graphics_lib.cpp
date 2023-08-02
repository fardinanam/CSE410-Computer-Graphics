#include "graphics_lib.h"
#include "matrix.h"
#include "vector3D.h"
#include "bitmap_image.hpp"
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
Scene::Scene(const std::string inputFilename, const std::string configFilename) {
    this->inputFilename = inputFilename;
    this->configFilename = configFilename;

    m = Matrix(4, 4);
    rightDir = Vector3D(1, 0, 0);
    lookDir = Vector3D(0, 0, -1);
    upDir = Vector3D(0, 1, 0);
    
    for (int i = 0; i < 4; i++) {
        m.set(i, i, 1);
    }

    parseConfig();
    initializeZBuffer();
    initializeBitmapImg();
}

Scene::~Scene() {
    for (int i = 0; i < (int)screenHeight; i++) {
        delete[] zBuffer[i];
    }
    delete[] zBuffer;
}

/**
 * Parses the config file and sets the projection parameters.
*/
void Scene::parseConfig() {
    // read from config file
    std::ifstream configFile(configFilename);
    std::string line;

    // read the first line
    std::getline(configFile, line);
    std::istringstream iss(line);
    iss >> screenWidth >> screenHeight;
}

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
 * models the basic transformations of the scene 
 * (without considering eye position and perspective) 
 * and saves the transformed points in stage 1
*/
void Scene::modelTransformation() {
    // read the file line by line
    std::ifstream sceneFile(inputFilename);
    std::ofstream stage1File(stage1Filename);
    std::string line;
    double eyeX, eyeY, eyeZ;
    double lookX, lookY, lookZ;
    double upX, upY, upZ;

    int lineno = 0;
    while (std::getline(sceneFile, line)) {
        lineno++;

        if (lineno == 1) {
            // parse eye position
            std::istringstream iss(line);
            iss >> eyeX >> eyeY >> eyeZ;
        } else if (lineno == 2) {
            // parse look at position
            std::istringstream iss(line);
            iss >> lookX >> lookY >> lookZ;
        } else if (lineno == 3) {
            // parse up direction
            std::istringstream iss(line);
            iss >> upX >> upY >> upZ;

            // set the view matrix
            lookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);
        } else if (lineno == 4) {
            // parse the projection parameters
            std::istringstream iss(line);
            iss >> fovY >> aspectRatio >> near >> far;
        } else if (line == "triangle") {
            std::vector<Vector3D> vertices;

            for (int i = 0; i < 3 && std::getline(sceneFile, line); i++) {
                // read the coordinates of the triangle
                std::istringstream iss(line);
                double x, y, z;
                iss >> x >> y >> z;

                // transform the coordinates
                Vector3D v = Vector3D(x, y, z);
                v = Vector3D(m * v.toHomogeneousMatrix());

                // print the coordinates to the stage 1 file
                stage1File << std::fixed << std::setprecision(7) << v.getX() << " "
                        << v.getY() << " " << v.getZ() << std::endl;
                
                vertices.push_back(v);
            }

            Vector3D color(rand() % 256, rand() % 256, rand() % 256);
            polygons.push_back(make_pair(vertices, color));

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

    // transform all the points of stage 1 with the new 
    // view matrix and save them in stage 2
    std::ofstream stage2File(stage2Filename);
    std::vector<std::pair<std::vector<Vector3D>, Vector3D>> stage2Polygons;

    for (auto polygon : polygons) {
        std::vector<Vector3D> vertices = polygon.first;
        Vector3D color = polygon.second;
        std::vector<Vector3D> transformedVertices;

        for (Vector3D v : vertices) {
            v = Vector3D(V * v.toHomogeneousMatrix());

            stage2File << std::fixed << std::setprecision(7) << v.getX() << " " << v.getY() << " " << v.getZ() << std::endl;

            transformedVertices.push_back(v);
        }

        stage2File << std::endl;
        stage2Polygons.push_back(make_pair(transformedVertices, color));
    }

    stage2File.close();
    polygons = stage2Polygons;
}

/**
 * Transforms the projection of the scene by multiplying the current projection transformation matrix
 * and saves the transformed points in stage 3
*/
void Scene::transformProjection() {
    double fovX = fovY * aspectRatio;
    double t = near * tan(fovY * M_PI / 360.0);
    double r = near * tan(fovX * M_PI / 360.0);

    Matrix P = Matrix(4, 4);
    P.set(0, 0, near / r);
    P.set(1, 1, near / t);
    P.set(2, 2, -(far + near) / (far - near));
    P.set(2, 3, -(2 * far * near) / (far - near));
    P.set(3, 2, -1);

    std::ofstream stage3File(stage3Filename);
    std::vector<std::pair<std::vector<Vector3D>, Vector3D>> stage3Polygons;

    for(auto polygon : polygons) {
        std::vector<Vector3D> vertices = polygon.first;
        Vector3D color = polygon.second;
        std::vector<Vector3D> transformedVertices;

        for (Vector3D v : vertices) {
            v = Vector3D(P * v.toHomogeneousMatrix());

            stage3File << std::fixed << std::setprecision(7) << v.getX() << " " << v.getY() << " " << v.getZ() << std::endl;

            transformedVertices.push_back(v);
        }

        stage3File << std::endl;
        stage3Polygons.push_back(make_pair(transformedVertices, color));
    }

    stage3File.close();
    polygons = stage3Polygons;
}

void Scene::initializeZBuffer() {
    zMax = zBackLimit;
    dx = (zBufferRightLimit - zBufferLeftLimit) / screenWidth;
    dy = (zBufferTopLimit - zBufferBottomLimit) / screenHeight;

    topY = zBufferTopLimit - dy / 2;
    bottomY = zBufferBottomLimit + dy / 2;
    leftX = zBufferLeftLimit + dx / 2;
    rightX = zBufferRightLimit - dx / 2;

    zBuffer = new double*[(int)screenHeight];
    for (int i = 0; i < (int)screenHeight; i++) {
        zBuffer[i] = new double[(int)screenWidth];
        for (int j = 0; j < (int)screenWidth; j++) {
            zBuffer[i][j] = zMax;
        }
    }
}

void Scene::initializeBitmapImg() {
    image.setwidth_height(screenWidth, screenHeight);

    // set background to black
    image.set_all_channels(0, 0, 0);
}

void calculateEquationOfPlane(Vector3D v1, Vector3D v2, Vector3D v3, double &A, double &B, double &C, double &D) {
    Vector3D normal = ((v2 - v1).cross(v3 - v1)).normalize();
    A = normal.getX();
    B = normal.getY();
    C = normal.getZ();
    D = -(A * v1.getX() + B * v1.getY() + C * v1.getZ());
}

void Scene::transformToZBuffer() {
    for (auto polygon : polygons) {
        std::vector<Vector3D> vertices = polygon.first;
        Vector3D color = polygon.second;

        // calculate the equation of the plane of the polygon
        if (vertices.size() < 3) {
            throw std::invalid_argument("Polygon must have at least 3 vertices");
        }

        double A, B, C, D;
        calculateEquationOfPlane(vertices[0], vertices[1], vertices[2], A, B, C, D);

        // find the top and bottom scanline
        double minY = vertices[0].getY();
        double maxY = vertices[0].getY();
        
        for (Vector3D v : vertices) {
            double vy = v.getY();
            if (vy > maxY) {
                maxY = vy;
            }
            if (vy < minY) {
                minY = vy;
            }
        }

        // clip y values to the z-buffer limits
        double topScanline = maxY < topY ? maxY : topY;
        double bottomScanline = minY > bottomY ? minY : bottomY; 

        // for row_no from top_scanline to bottom_scanline
        // top row is indexed 0

        int rowNo = int((topY - topScanline) / dy);
        int bottomRowNo = int((topY - bottomScanline) / dy);

        for(;rowNo <= bottomRowNo; rowNo++) {
            // Find left_intersecting_column and right_intersecting_column
            // use geometry to find the intersecting points
            double y = topY - rowNo * dy;

            double minX = rightX;
            double maxX = leftX;

            for (int i = 0; i < vertices.size(); i++) {
                int j = (i + 1) % vertices.size();
                double x1 = vertices[i].getX();
                double y1 = vertices[i].getY();
                double x2 = vertices[j].getX();
                double y2 = vertices[j].getY();
                
                // if the line is entirely above or below the scanline, ignore
                if ((y1 > y && y2 > y) || (y1 < y && y2 < y)) {
                    continue;
                }

                if (y1 != y2) {
                    double x = x1 + (x1 - x2) * ((y - y1) / (y1 - y2));

                    if (x > maxX) {
                        maxX = x;
                    }
                    if (x < minX) {
                        minX = x;
                    }
                } else {
                    if (x2 > maxX) {
                        maxX = x2;
                    }
                    if (x2 < minX) {
                        minX = x2;
                    }
                }
            }
            
            // clip x values to z-buffer limits
            double leftIntersectingColumn = minX > leftX ? minX : leftX;
            double rightIntersectingColumn = maxX < rightX ? maxX : rightX;

            // for col_no from left_intersecting_column to right_intersecting_column
            int colNo = (int)((leftIntersectingColumn - leftX) / dx) + 1;
            int rightColNo = (int)((rightIntersectingColumn - leftX) / dx);

            for(;colNo <= rightColNo; colNo++) {
                // Calculate z values using geometry
                double x = leftX + colNo * dx;
                double z = -(A * x + B * y + D) / C;
                

                // Compare with z-buffer and z_front_limit and update if required
                if (z < zFrontLimit) {
                    continue;
                }
                if (z < zBuffer[rowNo][colNo]) {
                    zBuffer[rowNo][colNo] = z;
                    // Update pixel information
                    image.set_pixel(colNo, rowNo, color.getX(),
                        color.getY(), color.getZ());
                }
            }
        }

    }

    image.save_image(imageFileName);
}

void Scene::writeZBufferToFile() {
    std::ofstream zBufferFile;
    zBufferFile.open("zBuffer.txt");
    for (int i = 0; i < (int)screenHeight; i++) {
        for (int j = 0; j < (int)screenWidth; j++) {
            if (zBuffer[i][j] < zMax) {
                zBufferFile << zBuffer[i][j] << "\t";
            }
        }
        zBufferFile << std::endl;
    }
    zBufferFile.close();
}

/**
 * Reads the scene from input file and executes the commands.
 */
void Scene::draw() {
    modelTransformation();
    transformView();
    transformProjection();
    transformToZBuffer();
    writeZBufferToFile();
}