#include "vector3D.h"
#include <cmath>

Vector3D::Vector3D() {
    mat = Matrix(4, 1);
    mat.set(3, 0, 1);
}

Vector3D::Vector3D(double x, double y, double z) : Vector3D() {
    mat.set(0, 0, x);
    mat.set(1, 0, y);
    mat.set(2, 0, z);
    mat.set(3, 0, 1);
}

Vector3D::Vector3D(const Vector3D &v) {
    mat = v.mat;
}

Vector3D::~Vector3D() {}

double Vector3D::getX() const {
    return mat.get(0, 0);
}

double Vector3D::getY() const {
    return mat.get(1, 0);
}

double Vector3D::getZ() const {
    return mat.get(2, 0);
}

void Vector3D::setX(double x) {
    mat.set(0, 0, x);
}

void Vector3D::setY(double y) {
    mat.set(1, 0, y);
}

void Vector3D::setZ(double z) {
    mat.set(2, 0, z);
}

Matrix Vector3D::toMatrix() const {
    return mat;
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