#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include "parser.hpp"

#define INIT_WINDOW_WIDTH 700
#define INIT_WINDOW_HEIGHT 700
#define INIT_WINDOW_X 100
#define INIT_WINDOW_Y 100

using namespace std;

// config
DescriptionParser parser("description.txt");

// Global variables
struct point eye;
struct point lookAtDir;
struct point upDir;
struct point rightDir;

const GLfloat rotationAmount = 10;
const GLfloat cameraMoveAmount = 1;
GLfloat rotationAngle = 0.0f;

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

  description d = parser.getViewDescription();
  gluPerspective(d.fovY, d.aspectRatio, d.near, d.far);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(
    eye.x, eye.y, eye.z,
    eye.x + lookAtDir.x, eye.y + lookAtDir.y, eye.z + lookAtDir.z,
    upDir.x, upDir.y, upDir.z
  );

  glRotated(rotationAngle, 0, 1, 0);

  drawAxes(20.0);
  vector<Object*> objects = parser.getObjects();
  for (int i = 0; i < objects.size(); i++) {
    objects[i]->draw();
  }

  glutSwapBuffers();
}



int main(int argc, char **argv) {
  eye = {0, 30, 70};
  lookAtDir = {0, -30, -70};
  upDir = {0, 7/sqrt(58), 3/sqrt(58)};
  rightDir = {1, 0, 0};

  parser.parse();
  parser.printDescription();

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