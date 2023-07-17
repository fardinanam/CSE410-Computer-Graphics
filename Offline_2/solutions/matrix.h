#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

class Matrix {
private:
    int row, col;
    double **mat;

public:
    Matrix();
    Matrix(int row, int col);
    Matrix(const Matrix &m);
    ~Matrix();

    int getRow() const;
    int getCol() const;
    double get(int i, int j) const;
    void set(int i, int j, double val);

    double determinant() const;
    double cofactor(int, int) const;
    Matrix adjoint() const;
    Matrix transpose() const;
    Matrix inverse() const;
    Matrix normalize() const;

    Matrix operator+(const Matrix &m) const;
    Matrix operator-(const Matrix &m) const;
    Matrix operator*(const Matrix &m) const;
    Matrix operator*(double val) const;
    Matrix operator/(double val) const;
    Matrix operator-() const;

    Matrix &operator=(const Matrix &m);

    bool operator==(const Matrix &m) const;
    bool operator!=(const Matrix &m) const;

    friend std::ostream &operator<<(std::ostream &out, const Matrix &m);
};

#endif