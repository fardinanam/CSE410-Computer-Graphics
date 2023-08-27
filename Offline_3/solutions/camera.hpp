#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vector.hpp"

class Camera {
private:
  Vector position;
  Vector lookAtPos;
  Vector upDir;

  void setUpDir();
public:
  Camera();
  Camera(Vector position, Vector lookAtPos, Vector upDir);
  Vector getPosition();
  Vector getLookAtPos();
  Vector getLookDir();
  Vector getUpDir();
  Vector getRightDir();

  void moveForward(double distance);
  void moveBackward(double distance);
  void moveLeft(double distance);
  void moveRight(double distance);
  void moveUp(double distance);
  void moveDown(double distance);
  void lookLeft(double angle);
  void lookRight(double angle);
  void lookUp(double angle);
  void lookDown(double angle);
};

Camera::Camera() {
  this->position = Vector(0, 0, 0);
  this->lookAtPos = Vector(0, 0, 1);
  this->upDir = Vector(0, 1, 0);
}

Camera::Camera(Vector position, Vector lookAtPos, Vector upDir) {
  this->position = position;
  this->lookAtPos = lookAtPos;
  this->upDir = upDir.normalize();
  setUpDir();
}

void Camera::setUpDir() {
  this->upDir = getRightDir().cross(getLookDir()).normalize();
}

Vector Camera::getPosition() {
  return this->position;
}

Vector Camera::getLookAtPos() {
  return this->lookAtPos;
}

Vector Camera::getLookDir() {
  return (this->lookAtPos - this->position).normalize();
}

Vector Camera::getUpDir() {
  return this->upDir;
}

Vector Camera::getRightDir() {
  return getLookDir().cross(this->upDir).normalize();
}

void Camera::moveForward(double distance) {
  Vector lookAtDir = getLookDir();
  this->position = this->position + lookAtDir * distance;
  this->lookAtPos = this->lookAtPos + lookAtDir * distance;
}

void Camera::moveBackward(double distance) {
  Vector lookAtDir = getLookDir();
  this->position = this->position - lookAtDir * distance;
  this->lookAtPos = this->lookAtPos - lookAtDir * distance;
}

void Camera::moveLeft(double distance) {
  Vector rightDir = getRightDir();
  this->position = this->position - rightDir * distance;
  this->lookAtPos = this->lookAtPos - rightDir * distance;
}

void Camera::moveRight(double distance) {
  Vector rightDir = getRightDir();
  this->position = this->position + rightDir * distance;
  this->lookAtPos = this->lookAtPos + rightDir * distance;
}

void Camera::moveUp(double distance) {
  Vector upDir = getUpDir();
  this->position = this->position + upDir * distance;
  this->lookAtPos = this->lookAtPos + upDir * distance;
}

void Camera::moveDown(double distance) {
  Vector upDir = getUpDir();
  this->position = this->position - upDir * distance;
  this->lookAtPos = this->lookAtPos - upDir * distance;
}

void Camera::lookLeft(double angle) {
  Vector lookAtDir = getLookDir();
  Vector newLookAtDir = lookAtDir.rotate(this->upDir, angle);
  this->lookAtPos = this->position + newLookAtDir;
}

void Camera::lookRight(double angle) {
  lookLeft(-angle);
}

void Camera::lookUp(double angle) {
  Vector lookAtDir = getLookDir();
  Vector newLookAtDir = lookAtDir.rotate(getRightDir(), angle);
  this->lookAtPos = this->position + newLookAtDir;
}

void Camera::lookDown(double angle) {
  lookUp(-angle);
}

#endif // CAMERA_HPP