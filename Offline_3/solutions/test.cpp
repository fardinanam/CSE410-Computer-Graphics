#include "camera.hpp"
#include "vector.hpp"
#include "object.hpp"
#include "sphere.hpp"
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

int main() {
  // Camera camera(Vector(0, 0, 100), Vector(0, 0, 0), Vector(0, 1, 0));
  // camera.setAspectRatio(1);
  // camera.setFovY(80);
  // camera.setPixelsY(768);
  // camera.setNearZ(1);
  // camera.setFarZ(1000);
  
  // cout << camera << endl;

  // vector<Object*> objects;
  // objects.push_back(new Sphere(Vector(0, 0, 0), 10, {1, 0, 0}, 0.1, 0.9, 0.1, 0.1, 0.1));
  
  // camera.capture(objects, 0);
  // cout << camera.pixelToVect(0, 0) << endl;

  int x = 51;
  int y = -51;

  cout << x % 50 << endl;
  cout << y % 50 << endl;
}