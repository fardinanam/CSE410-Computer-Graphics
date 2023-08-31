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
const Vector defaultEye(0, -145, 40);
const Vector defaultLookAtPos(0, 0, 40);
const Vector defaultUpDir(0, 0, 1);
bool isShowingAxes = true;

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
      camera.lookLeft(cameraRotationAmount);
      break;
    case 'd':
      camera.lookRight(cameraRotationAmount);
      break;
    case 'w':
      camera.lookUp(cameraRotationAmount);
      break;
    case 's':
      camera.lookDown(cameraRotationAmount);
      break;
    case 'q':
      camera.tiltLeft(cameraRotationAmount);
      break;
    case 'e':
      camera.tiltRight(cameraRotationAmount);
      break;
    case '1':
      camera.lookLeft(cameraRotationAmount);
      break;
    case '2':
      camera.lookRight(cameraRotationAmount);
      break;
    case '3':
      camera.lookUp(cameraRotationAmount);
      break;
    case '4':
      camera.lookDown(cameraRotationAmount);
      break;
    case '5':
      camera.tiltLeft(cameraRotationAmount);
      break;
    case '6':
      camera.tiltRight(cameraRotationAmount);
      break;
    case '0':
      camera.capture(parser.getObjects(), parser.getLights()
        , parser.getViewDescription().levelOfRecursion);
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
      camera.moveForward(cameraRotationAmount);
      break;
    case GLUT_KEY_DOWN:
      camera.moveBackward(cameraRotationAmount);
      break;

    case GLUT_KEY_RIGHT:
      camera.moveRight(cameraRotationAmount);
      break;
    case GLUT_KEY_LEFT:
      camera.moveLeft(cameraRotationAmount);
      break;
    case GLUT_KEY_PAGE_UP:
      camera.moveUp(cameraMoveAmount);
      break;
    case GLUT_KEY_PAGE_DOWN:
      camera.moveDown(cameraMoveAmount);
      break;
    case GLUT_KEY_HOME:
      camera.setCamera(defaultEye, defaultLookAtPos, defaultUpDir);
      break;
    default:
      return;
  }

  glutPostRedisplay(); // Post a paint request to activate display()
}

void mouseListener(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // Pause/resume
    isShowingAxes = !isShowingAxes;
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

void drawCone(Vector position, Vector direction, double cutOffAngle, GLfloat height, int slices, int stacks)
{
  direction = direction.normalize();
  double theta = cutOffAngle * M_PI / 180;
  // Draw the cone's triangles.
  glPushMatrix();
  for (int i = 0; i < stacks; i++) {
    double currentHeight = i * height / stacks;
    Vector center = position + direction * currentHeight;
    double radius = currentHeight * tan(theta);
    // draw circles at the center of each stack
    // so that the cone looks smooth and facing towards the lookAt position
    // circle is made of slices triangles
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(center.x, center.y, center.z);
    for (int j = 0; j <= slices; j++) {
      double angle = j * 2 * M_PI / slices;
      double x = center.x + radius * cos(angle);
      double y = center.y + radius * sin(angle);
      glVertex3f(x, y, center.z);
    }
    glEnd();
  }
  glPopMatrix();
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

  if (isShowingAxes)
    drawAxes(1000.0);
  
  vector<Object*> objects = parser.getObjects();
  for (int i = 0; i < objects.size(); i++) {
    objects[i]->draw();
  }

  parser.getCheckerboard().draw(camera.getPosition());

  vector<Light> lights = parser.getLights();
  glColor3f(0.2, 0.2, 0.2);
  for (Light light : lights) {
    glPushMatrix();
    if (light.cutOffAngle != 360) {
      Vector lookDir = (light.lookAt - light.position).normalize();
      glColor3f(0.45, 0.45, .5);
      drawCone(light.position, lookDir, light.cutOffAngle, 15, 100, 50);

    } else {      
    glTranslatef(light.position.x, light.position.y, light.position.z);
      glutSolidSphere(3, 50, 50);
    }
    
    glPopMatrix();
  }
  
  glutSwapBuffers();
}



int main(int argc, char **argv) {
  camera = Camera(defaultEye, defaultLookAtPos, defaultUpDir);

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
  glutMouseFunc(mouseListener);
  initGL();
  glutMainLoop();
  return 0;
}