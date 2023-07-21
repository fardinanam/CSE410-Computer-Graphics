#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "matrix.h"

class Vector3D {
private:
    Matrix mat;
public:
    Vector3D();
    Vector3D(double x, double y, double z);
    Vector3D(const Matrix &m);
    Vector3D(const Vector3D &v);
    ~Vector3D();

    double getX() const;
    double getY() const;
    double getZ() const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);

    Matrix toMatrix() const;

    double magnitude() const;
    Vector3D normalize() const;
    Vector3D cross(const Vector3D &v) const;
    double dot(const Vector3D &v) const;

    Vector3D operator+(const Vector3D &v) const;
    Vector3D operator-(const Vector3D &v) const;
    Vector3D operator*(double s) const;
    Vector3D operator/(double s) const;
    Vector3D operator=(const Matrix &m);

    friend Vector3D operator*(double s, const Vector3D &v);
    friend Vector3D operator/(double &s, const Vector3D &v);
    
    friend std::ostream &operator<<(std::ostream &out, const Vector3D &v);
};

#endif // VECTOR3D_H