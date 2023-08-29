#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include "camera.hpp"
#include "parser.hpp"
#include "utils.hpp"

#define INIT_WINDOW_WIDTH 700
#define INIT_WINDOW_HEIGHT 700
#define INIT_WINDOW_X 100
#define INIT_WINDOW_Y 100

using namespace std;

// config
DescriptionParser parser("description.txt");

// Global variables
Camera camera;

const GLfloat cameraRotationAmount = 10;
const GLfloat cameraMoveAmount = 5;
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
  gluPerspective(camera.getFovY(), camera.getAspectRatio(), camera.getNearZ(),
    camera.getFarZ());
}

void keyboardListener(unsigned char key, int x, int y) {
  switch (key) {
    case 'a':
      camera.moveLeft(cameraMoveAmount);
      break;
    case 'd':
      camera.moveRight(cameraMoveAmount);
      break;
    case 'w':
      camera.moveForward(cameraMoveAmount);
      break;
    case 's':
      camera.moveBackward(cameraMoveAmount);
      break;
    case '1':
      camera.moveUp(cameraMoveAmount);
      break;
    case '2':
      camera.moveDown(cameraMoveAmount);
      break;
    case '0':
      camera.capture(parser.getObjects(), parser.getNormalLights()
        , parser.getSpotLights(), parser.getViewDescription().levelOfRecursion);
      break;
    default:
      return;
  }
  glutPostRedisplay(); // Post a paint request to activate display()
}

void specialKeyListener(int key, int x, int y)
{
  switch (key) {
    case GLUT_KEY_UP:
      camera.lookUp(cameraRotationAmount);
      break;
    case GLUT_KEY_DOWN:
      camera.lookDown(cameraRotationAmount);
      break;

    case GLUT_KEY_RIGHT:
      camera.lookRight(cameraRotationAmount);
      break;
    case GLUT_KEY_LEFT:
      camera.lookLeft(cameraRotationAmount);
      break;
    case GLUT_KEY_PAGE_UP:
      camera.moveUp(cameraMoveAmount);
      break;
    case GLUT_KEY_PAGE_DOWN:
      camera.moveDown(cameraMoveAmount);
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

  gluPerspective(camera.getFovY(), camera.getAspectRatio(), camera.getNearZ(), camera.getFarZ());
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(
    camera.getPosition().x, camera.getPosition().y, camera.getPosition().z,
    camera.getLookAtPos().x, camera.getLookAtPos().y, camera.getLookAtPos().z,
    camera.getUpDir().x, camera.getUpDir().y, camera.getUpDir().z
  );

  glRotated(rotationAngle, 0, 1, 0);

  drawAxes(1000.0);
  vector<Object*> objects = parser.getObjects();
  for (int i = 0; i < objects.size(); i++) {
    objects[i]->draw();
  }

  parser.getCheckerboard().draw(camera.getPosition());

  vector<normalLight> normalLights = parser.getNormalLights();

  for (int i = 0; i < normalLights.size(); i++) {
    // cout << "Normal light " << i << ": " << normalLights[i].position.x << " " << normalLights[i].position.y << " " << normalLights[i].position.z << endl;
    glPushMatrix();
    glTranslatef(normalLights[i].position.x, normalLights[i].position.y, normalLights[i].position.z);
    glColor3f(0.5, 0.5, 0.5);
    glutSolidSphere(1, 20, 20);
    glPopMatrix();
  }

  glutSwapBuffers();
}



int main(int argc, char **argv) {
  Vector eye = {0, 10, 100};
  Vector lookAtPos(0, 0, 0);
  Vector upDir(0, 1, 0);

  camera = Camera(eye, lookAtPos, upDir);


  parser.parse();
  parser.printDescription();

  description d = parser.getViewDescription();

  camera.setNearZ(d.near);
  camera.setFarZ(d.far);
  camera.setAspectRatio(d.aspectRatio);
  camera.setFovY(d.fovY);
  camera.setPixelsY(d.numPixelsY);

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