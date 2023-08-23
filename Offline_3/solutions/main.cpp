#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "utils.hpp"

#define INIT_WINDOW_WIDTH 700
#define INIT_WINDOW_HEIGHT 700
#define INIT_WINDOW_X 100
#define INIT_WINDOW_Y 100

using namespace std;

// Global variables
struct point eye;
struct point lookAtDir;
struct point upDir;
struct point rightDir;

const GLfloat rotationAmount = 10;
const GLfloat cameraMoveAmount = 1;
GLfloat rotationAngle = 0.0f;

GLfloat fovY = 80.0f;
GLfloat aspectRatio = 1.0f;
GLfloat zNear = 1.0f;
GLfloat zFar = 1000.0f;

void initGL() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

void reshapeListener(GLsizei width, GLsizei height) {
  // Compute aspect ratio of the new window
  if (height == 0)
    height = 1; // To prevent divide by 0
  GLfloat aspect = (GLfloat)width / (GLfloat)height;

  // Set the viewport to cover the new window
  glViewport(0, 0, width, height);

  // Set the aspect ratio of the clipping area to match the viewport
  glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
  glLoadIdentity();            // Reset the projection matrix
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void rotateClockwiseY() {
  rotationAngle += rotationAmount;
}

void rotateAntiClockwiseY() {
  rotationAngle -= rotationAmount;
}

void lookLeft() {
  rightDir.x = rightDir.x * cos(cameraMoveAmount) + lookAtDir.x * sin(cameraMoveAmount);
  rightDir.y = rightDir.y * cos(cameraMoveAmount) + lookAtDir.y * sin(cameraMoveAmount);
  rightDir.z = rightDir.z * cos(cameraMoveAmount) + lookAtDir.z * sin(cameraMoveAmount);

  lookAtDir.x = lookAtDir.x * cos(cameraMoveAmount) - rightDir.x * sin(cameraMoveAmount);
  lookAtDir.y = lookAtDir.y * cos(cameraMoveAmount) - rightDir.y * sin(cameraMoveAmount);
  lookAtDir.z = lookAtDir.z * cos(cameraMoveAmount) - rightDir.z * sin(cameraMoveAmount);
}

void lookRight() {
  rightDir.x = rightDir.x * cos(-cameraMoveAmount) + lookAtDir.x * sin(-cameraMoveAmount);
  rightDir.y = rightDir.y * cos(-cameraMoveAmount) + lookAtDir.y * sin(-cameraMoveAmount);
  rightDir.z = rightDir.z * cos(-cameraMoveAmount) + lookAtDir.z * sin(-cameraMoveAmount);

  lookAtDir.x = lookAtDir.x * cos(-cameraMoveAmount) - rightDir.x * sin(-cameraMoveAmount);
  lookAtDir.y = lookAtDir.y * cos(-cameraMoveAmount) - rightDir.y * sin(-cameraMoveAmount);
  lookAtDir.z = lookAtDir.z * cos(-cameraMoveAmount) - rightDir.z * sin(-cameraMoveAmount);
}

void lookUp() {
  lookAtDir.x = lookAtDir.x * cos(cameraMoveAmount) + upDir.x * sin(cameraMoveAmount);
  lookAtDir.y = lookAtDir.y * cos(cameraMoveAmount) + upDir.y * sin(cameraMoveAmount);
  lookAtDir.z = lookAtDir.z * cos(cameraMoveAmount) + upDir.z * sin(cameraMoveAmount);

  upDir.x = upDir.x * cos(cameraMoveAmount) - lookAtDir.x * sin(cameraMoveAmount);
  upDir.y = upDir.y * cos(cameraMoveAmount) - lookAtDir.y * sin(cameraMoveAmount);
  upDir.z = upDir.z * cos(cameraMoveAmount) - lookAtDir.z * sin(cameraMoveAmount);
}

void lookDown() {
  lookAtDir.x = lookAtDir.x * cos(-cameraMoveAmount) + upDir.x * sin(-cameraMoveAmount);
  lookAtDir.y = lookAtDir.y * cos(-cameraMoveAmount) + upDir.y * sin(-cameraMoveAmount);
  lookAtDir.z = lookAtDir.z * cos(-cameraMoveAmount) + upDir.z * sin(-cameraMoveAmount);

  upDir.x = upDir.x * cos(-cameraMoveAmount) - lookAtDir.x * sin(-cameraMoveAmount);
  upDir.y = upDir.y * cos(-cameraMoveAmount) - lookAtDir.y * sin(-cameraMoveAmount);
  upDir.z = upDir.z * cos(-cameraMoveAmount) - lookAtDir.z * sin(-cameraMoveAmount);
}

void tiltCounterClockwise() {
  upDir.x = upDir.x * cos(cameraMoveAmount) + rightDir.x * sin(cameraMoveAmount);
  upDir.y = upDir.y * cos(cameraMoveAmount) + rightDir.y * sin(cameraMoveAmount);
  upDir.z = upDir.z * cos(cameraMoveAmount) + rightDir.z * sin(cameraMoveAmount);

  rightDir.x = rightDir.x * cos(cameraMoveAmount) - upDir.x * sin(cameraMoveAmount);
  rightDir.y = rightDir.y * cos(cameraMoveAmount) - upDir.y * sin(cameraMoveAmount);
  rightDir.z = rightDir.z * cos(cameraMoveAmount) - upDir.z * sin(cameraMoveAmount);
}

void tiltClockwise() {
  upDir.x = upDir.x * cos(-cameraMoveAmount) + rightDir.x * sin(-cameraMoveAmount);
  upDir.y = upDir.y * cos(-cameraMoveAmount) + rightDir.y * sin(-cameraMoveAmount);
  upDir.z = upDir.z * cos(-cameraMoveAmount) + rightDir.z * sin(-cameraMoveAmount);

  rightDir.x = rightDir.x * cos(-cameraMoveAmount) - upDir.x * sin(-cameraMoveAmount);
  rightDir.y = rightDir.y * cos(-cameraMoveAmount) - upDir.y * sin(-cameraMoveAmount);
  rightDir.z = rightDir.z * cos(-cameraMoveAmount) - upDir.z * sin(-cameraMoveAmount);
}

void moveForward() {
  eye.x += lookAtDir.x * cameraMoveAmount;
  eye.y += lookAtDir.y * cameraMoveAmount;
  eye.z += lookAtDir.z * cameraMoveAmount;
}

void moveBackward() {
  eye.x -= lookAtDir.x * cameraMoveAmount;
  eye.y -= lookAtDir.y * cameraMoveAmount;
  eye.z -= lookAtDir.z * cameraMoveAmount;
}

void moveLeft() {
  eye.x += rightDir.x;
  eye.y += rightDir.y;
  eye.z += rightDir.z;
}

void moveRight() {
  eye.x -= rightDir.x;
  eye.y -= rightDir.y;
  eye.z -= rightDir.z;
}

void moveUp() {
  eye.x += upDir.x;
  eye.y += upDir.y;
  eye.z += upDir.z;
}

void moveDown() {
  eye.x -= upDir.x;
  eye.y -= upDir.y;
  eye.z -= upDir.z;
}

void moveUpWithoutChangingReference() {
  struct point lookAtPoint;

  lookAtPoint.x = eye.x + lookAtDir.x;
  lookAtPoint.y = eye.y + lookAtDir.y;
  lookAtPoint.z = eye.z + lookAtDir.z;

  eye.x += upDir.x * cameraMoveAmount;
  eye.y += upDir.y * cameraMoveAmount;
  eye.z += upDir.z * cameraMoveAmount;

  lookAtDir.x = lookAtPoint.x - eye.x;
  lookAtDir.y = lookAtPoint.y - eye.y;
  lookAtDir.z = lookAtPoint.z - eye.z;
}

void moveDownWithoutChangingReference() {
  struct point lookAtPoint;

  lookAtPoint.x = eye.x + lookAtDir.x;
  lookAtPoint.y = eye.y + lookAtDir.y;
  lookAtPoint.z = eye.z + lookAtDir.z;

  eye.x -= upDir.x * cameraMoveAmount;
  eye.y -= upDir.y * cameraMoveAmount;
  eye.z -= upDir.z * cameraMoveAmount;

  lookAtDir.x = lookAtPoint.x - eye.x;
  lookAtDir.y = lookAtPoint.y - eye.y;
  lookAtDir.z = lookAtPoint.z - eye.z;
}

void keyboardListener(unsigned char key, int x, int y) {
  switch (key) {
    case '1':
      lookLeft();
      break;

    case '2':
      lookRight();
      break;

    case '3':
      lookUp();
      break;

    case '4':
      lookDown();
      break;

    case '5':
      tiltCounterClockwise();
      break;

    case '6':
      tiltClockwise();
      break;

    case 'a':
      rotateClockwiseY();
      break;

    case 'd':
      rotateAntiClockwiseY();
      break;

    case 'w':
      moveUpWithoutChangingReference();
      break;

    case 's':
      moveDownWithoutChangingReference();
      break;


    default:
      return;
  }
  glutPostRedisplay(); // Post a paint request to activate display()
}

void specialKeyListener(int key, int x, int y)
{
  switch (key) {
    case GLUT_KEY_UP: // down arrow key
      moveForward();
      break;
    case GLUT_KEY_DOWN: // up arrow key
      moveBackward();
      break;

    case GLUT_KEY_RIGHT:
      moveLeft();
      break;
    case GLUT_KEY_LEFT:
      moveRight();
      break;

    case GLUT_KEY_PAGE_UP:
      moveUp();
      break;
    case GLUT_KEY_PAGE_DOWN:
      moveDown();
      break;

    default:
      return;
  }

  glutPostRedisplay(); // Post a paint request to activate display()
}

void drawAxes(GLfloat length = 1.0) {
  glPushMatrix();
  glLineWidth(3);
  glScaled(length, length, length);
  glBegin(GL_LINES);
  glColor3f(1, 0, 0); // Red

  // X axis
  glVertex3f(0, 0, 0);
  glVertex3f(1, 0, 0);

  glColor3f(0, 1, 0); // Green
  // Y axis
  glVertex3f(0, 0, 0);
  glVertex3f(0, 1, 0);

  glColor3f(0, 0, 1); // Blue
  // Z axis
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 1);
  glEnd();
  glPopMatrix();
}

void drawSphere(point center, GLfloat radius, point color = {1, 1, 1}) {
  glPushMatrix();
  glTranslatef(center.x, center.y, center.z);
  glColor3f(color.x, color.y, color.z);
  glutSolidSphere(radius, 100, 100);
  glPopMatrix();
}

void drawPyramid(point lowest, GLfloat width, GLfloat height, point color = {1, 1, 1}) {
  point center = {width/2, height, sqrt(3) * width / 4};
  
  glPushMatrix();
  glTranslatef(lowest.x, lowest.y, lowest.z);
  glColor3f(color.x, color.y, color.z);
  glBegin(GL_TRIANGLES);
  glVertex3f(0, 0, 0);
  glVertex3f(width, 0, 0);
  glVertex3f(center.x, center.y, center.z);

  glColor3f(0, 1, 0);

  glVertex3f(0, 0, 0);
  glVertex3f(width/2, 0, center.z * 2);
  glVertex3f(center.x, center.y, center.z);

  glColor3f(0, 0, 1);

  glVertex3f(width, 0, 0);
  glVertex3f(width/2, 0, center.z * 2);
  glVertex3f(center.x, center.y, center.z);

  glColor3f(1, 1, 0);

  glVertex3f(0, 0, 0);
  glVertex3f(width, 0, 0);
  glVertex3f(width/2, 0, center.z * 2);
  glEnd();
  glPopMatrix();
}

void drawCube(point bottomLowerLeft, GLfloat length, point color = {1, 1, 1}) {
  glPushMatrix();
  glTranslatef(bottomLowerLeft.x, bottomLowerLeft.y, bottomLowerLeft.z);
  glColor3f(color.x, color.y, color.z);
  glutSolidCube(length);
  glPopMatrix();
}

void drawInfiniteCheckerBoard(GLfloat widthOfEachCell) {
  int numberOfCells = 1000;

  GLfloat x = -numberOfCells * widthOfEachCell / 2;
  GLfloat y = 0;
  GLfloat z = -numberOfCells * widthOfEachCell / 2;

  for (int i = 0; i < numberOfCells; i++) {
    for (int j = 0; j < numberOfCells; j++) {
      if ((i + j) % 2 == 0) {
        glColor3f(1, 1, 1);
      } else {
        glColor3f(0, 0, 0);
      }
      glBegin(GL_QUADS);
      glVertex3f(x, y, z);
      glVertex3f(x + widthOfEachCell, y, z);
      glVertex3f(x + widthOfEachCell, y, z + widthOfEachCell);
      glVertex3f(x, y, z + widthOfEachCell);
      glEnd();
      x += widthOfEachCell;
    }
    x = -numberOfCells * widthOfEachCell / 2;
    z += widthOfEachCell;
  }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // TODO: change the hard coded values
  gluPerspective(fovY, aspectRatio, zNear, zFar);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(
    eye.x, eye.y, eye.z,
    eye.x + lookAtDir.x, eye.y + lookAtDir.y, eye.z + lookAtDir.z,
    upDir.x, upDir.y, upDir.z
  );

  glRotated(rotationAngle, 0, 1, 0);

  drawAxes(20.0);
  drawInfiniteCheckerBoard(50.0);
  drawSphere({20, 20, 20}, 20, {1, 1, 0});
  drawPyramid({-40, 0, 5.0}, 30, 40, {1, 0, 0});
  drawCube({-100, -100, 10}, 40, {0, 0.5, 1.0});

  glutSwapBuffers();
}

int main(int argc, char **argv) {
  eye = {0, 30, 70};
  lookAtDir = {0, -30, -70};
  upDir = {0, 7/sqrt(58), 3/sqrt(58)};
  rightDir = {1, 0, 0};

  // Initialize glut
  glutInit(&argc, argv);
  glutInitWindowSize(
    INIT_WINDOW_WIDTH, 
    INIT_WINDOW_HEIGHT
  );
  glutInitWindowPosition(
    INIT_WINDOW_X, 
    INIT_WINDOW_Y
  );

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Ray Tracer");
  glutDisplayFunc(display);
  glutReshapeFunc(reshapeListener);
  glutKeyboardFunc(keyboardListener);
  glutSpecialFunc(specialKeyListener);
  initGL();
  glutMainLoop();
  return 0;
}