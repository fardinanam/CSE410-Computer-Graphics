#include "vector3D.h"
#include "matrix.h"
#include <cmath>

Vector3D::Vector3D() {
    mat = Matrix(1, 3);
}

Vector3D::Vector3D(double x, double y, double z) : Vector3D() {
    mat.set(0, 0, x);
    mat.set(0, 1, y);
    mat.set(0, 2, z);
}

Vector3D::Vector3D(const Matrix &m) : Vector3D() {
    if (m.getRow() == 4 && m.getCol() == 1 && m.get(3, 0) == 1) {
        mat.set(0, 0, m.get(0, 0));
        mat.set(0, 1, m.get(1, 0));
        mat.set(0, 2, m.get(2, 0));
    } else if (m.getRow() == 3 && m.getCol() == 1) {
        mat = m.transpose(); 
    } else if (m.getCol() == 4 && m.getRow() == 1 && m.get(0, 3) == 1) {
        mat.set(0, 0, m.get(0, 0));
        mat.set(0, 1, m.get(0, 1));
        mat.set(0, 2, m.get(0, 2));
    } else if (m.getCol() == 3 && m.getRow() == 1) {
        mat = m;
    } else {
        throw std::invalid_argument("Matrix must be 3x1 or 1x3 or 4x1 or 1x4 with the last element (0, 3) or (3, 0) being 1");
    }
}

Vector3D::Vector3D(const Vector3D &v) {
    mat = v.mat;
}

Vector3D::~Vector3D() {}

double Vector3D::getX() const {
    return mat.get(0, 0);
}

double Vector3D::getY() const {
    return mat.get(0, 1);
}

double Vector3D::getZ() const {
    return mat.get(0, 2);
}

void Vector3D::setX(double x) {
    mat.set(0, 0, x);
}

void Vector3D::setY(double y) {
    mat.set(0, 1, y);
}

void Vector3D::setZ(double z) {
    mat.set(0, 2, z);
}

Matrix Vector3D::toMatrix() const {
    return mat;
}

/**
 * Returns the homogeneous matrix representation of the vector
*/
Matrix Vector3D::toHomogeneousMatrix() const {
    Matrix result(4, 1);
    result.set(0, 0, getX());
    result.set(1, 0, getY());
    result.set(2, 0, getZ());
    result.set(3, 0, 1);

    return result;
}

double Vector3D::magnitude() const {
    return sqrt(getX() * getX() + getY() * getY() + getZ() * getZ());
}

Vector3D Vector3D::normalize() const {
    double mag = magnitude();
    return Vector3D(getX() / mag, getY() / mag, getZ() / mag);
}

Vector3D Vector3D::cross(const Vector3D &v) const {
    double x = getY() * v.getZ() - getZ() * v.getY();
    double y = getZ() * v.getX() - getX() * v.getZ();
    double z = getX() * v.getY() - getY() * v.getX();
    return Vector3D(x, y, z);
}

double Vector3D::dot(const Vector3D &v) const {
    return getX() * v.getX() + getY() * v.getY() + getZ() * v.getZ();
}

Vector3D Vector3D::operator+(const Vector3D &v) const {
    return Vector3D(getX() + v.getX(), getY() + v.getY(), getZ() + v.getZ());
}

Vector3D Vector3D::operator-(const Vector3D &v) const {
    return Vector3D(getX() - v.getX(), getY() - v.getY(), getZ() - v.getZ());
}

Vector3D Vector3D::operator*(double s) const {
    return Vector3D(s * getX(), s * getY(), s * getZ());
}

Vector3D Vector3D::operator/(double s) const {
    return Vector3D(getX() / s, getY() / s, getZ() / s);
}

Vector3D Vector3D::operator=(const Matrix &m) {
    mat = m;
    return *this;
}

Vector3D operator*(double s, const Vector3D &v) {
    return Vector3D(v.getX() * s, v.getY() * s, v.getZ() * s);
}

Vector3D operator/(double &s, Vector3D &v) {
    return Vector3D(v.getX() / s, v.getY() / s, v.getZ() / s);
}

std::ostream &operator<<(std::ostream &out, const Vector3D &v) {
    out << "(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ")";
    return out;
}