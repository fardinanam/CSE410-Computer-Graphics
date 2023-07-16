#include<bits/stdc++.h>
#include "matrix.h"

using namespace std;

int main() {
    Matrix m(3, 3);
    m.set(0, 0, 2);
    m.set(0, 1, 5);
    m.set(0, 2, 6);
    m.set(1, 0, 4);
    m.set(1, 1, 5);
    m.set(1, 2, 6);
    m.set(2, 0, 7);
    m.set(2, 1, 8);
    m.set(2, 2, 9);

    cout << m.getRow() << " " << m.getCol() << endl;
    cout << m << endl;
    cout << "determinant:" << m.determinant() << endl;
    return 0;
}