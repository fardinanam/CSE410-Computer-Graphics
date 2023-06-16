#include <GL/glut.h>
#include <iostream>
#include <cmath>

#define CONVERSION_STEPS 16

struct point {
  GLfloat x, y, z;
};

// Global variables
const GLfloat rotationAmount = 10; 
const GLfloat cameraMoveAmount = 0.1;
GLfloat rotationAngle = 0;

struct point eyePos, relativeLookingPoint, upPos, rightDir;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;

const GLfloat baseTriangleCentroidX = 0.3333, baseTriangleCentroidY = 0.3333, baseTriangleCentroidZ = 0.3333; 

GLfloat baseTriangleCenterX = 0, baseTriangleCenterY = 0, baseTriangleCenterZ = 0;
GLfloat baseTriangleScale = 1.0;

GLfloat sphereFaceScale = 0;
GLfloat sphereTranslationX = 1.0, sphereTranslationY = 0.0, sphereTranslationZ = 0.0;

const GLfloat maxCylinderHeight = sqrt(2.0);
const GLfloat maxCylindeTheta = M_PI / 2.0;
const GLfloat cylinderMaxTranslationX = 1 / sqrt(2.0);
GLfloat cylinderTheta = maxCylindeTheta;
GLfloat cylinderHeight = maxCylinderHeight;
GLfloat cylinderTranslationX = cylinderMaxTranslationX;
GLfloat cylinderScale = 0;


GLfloat PHI = acos(-1 / 3), // angle between two faces of the octahedron
        radius = 0.577;     // radius of the sphere

bool isAxes = false, isOctahedron = true, isSphere = true;

/* Initialize OpenGL Graphics */
void initGL() {
  // Set "clearing" or background color
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
  glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
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

void lookLeft() {
  rightDir.x = rightDir.x * cos(cameraMoveAmount) + relativeLookingPoint.x * sin(cameraMoveAmount);
  rightDir.y = rightDir.y * cos(cameraMoveAmount) + relativeLookingPoint.y * sin(cameraMoveAmount);
  rightDir.z = rightDir.z * cos(cameraMoveAmount) + relativeLookingPoint.z * sin(cameraMoveAmount);

  relativeLookingPoint.x = relativeLookingPoint.x * cos(cameraMoveAmount) - rightDir.x * sin(cameraMoveAmount);
  relativeLookingPoint.y = relativeLookingPoint.y * cos(cameraMoveAmount) - rightDir.y * sin(cameraMoveAmount);
  relativeLookingPoint.z = relativeLookingPoint.z * cos(cameraMoveAmount) - rightDir.z * sin(cameraMoveAmount);
}

void lookRight() {
  rightDir.x = rightDir.x * cos(-cameraMoveAmount) + relativeLookingPoint.x * sin(-cameraMoveAmount);
  rightDir.y = rightDir.y * cos(-cameraMoveAmount) + relativeLookingPoint.y * sin(-cameraMoveAmount);
  rightDir.z = rightDir.z * cos(-cameraMoveAmount) + relativeLookingPoint.z * sin(-cameraMoveAmount);

  relativeLookingPoint.x = relativeLookingPoint.x * cos(-cameraMoveAmount) - rightDir.x * sin(-cameraMoveAmount);
  relativeLookingPoint.y = relativeLookingPoint.y * cos(-cameraMoveAmount) - rightDir.y * sin(-cameraMoveAmount);
  relativeLookingPoint.z = relativeLookingPoint.z * cos(-cameraMoveAmount) - rightDir.z * sin(-cameraMoveAmount);
}

void lookUp() {
  relativeLookingPoint.x = relativeLookingPoint.x * cos(cameraMoveAmount) + upPos.x * sin(cameraMoveAmount);
  relativeLookingPoint.y = relativeLookingPoint.y * cos(cameraMoveAmount) + upPos.y * sin(cameraMoveAmount);
  relativeLookingPoint.z = relativeLookingPoint.z * cos(cameraMoveAmount) + upPos.z * sin(cameraMoveAmount);

  upPos.x = upPos.x * cos(cameraMoveAmount) - relativeLookingPoint.x * sin(cameraMoveAmount);
  upPos.y = upPos.y * cos(cameraMoveAmount) - relativeLookingPoint.y * sin(cameraMoveAmount);
  upPos.z = upPos.z * cos(cameraMoveAmount) - relativeLookingPoint.z * sin(cameraMoveAmount);
}

void lookDown() {
  relativeLookingPoint.x = relativeLookingPoint.x * cos(-cameraMoveAmount) + upPos.x * sin(-cameraMoveAmount);
  relativeLookingPoint.y = relativeLookingPoint.y * cos(-cameraMoveAmount) + upPos.y * sin(-cameraMoveAmount);
  relativeLookingPoint.z = relativeLookingPoint.z * cos(-cameraMoveAmount) + upPos.z * sin(-cameraMoveAmount);

  upPos.x = upPos.x * cos(-cameraMoveAmount) - relativeLookingPoint.x * sin(-cameraMoveAmount);
  upPos.y = upPos.y * cos(-cameraMoveAmount) - relativeLookingPoint.y * sin(-cameraMoveAmount);
  upPos.z = upPos.z * cos(-cameraMoveAmount) - relativeLookingPoint.z * sin(-cameraMoveAmount);
}

void tiltCounterClockwise() {
  upPos.x = upPos.x * cos(cameraMoveAmount) + rightDir.x * sin(cameraMoveAmount);
  upPos.y = upPos.y * cos(cameraMoveAmount) + rightDir.y * sin(cameraMoveAmount);
  upPos.z = upPos.z * cos(cameraMoveAmount) + rightDir.z * sin(cameraMoveAmount);

  rightDir.x = rightDir.x * cos(cameraMoveAmount) - upPos.x * sin(cameraMoveAmount);
  rightDir.y = rightDir.y * cos(cameraMoveAmount) - upPos.y * sin(cameraMoveAmount);
  rightDir.z = rightDir.z * cos(cameraMoveAmount) - upPos.z * sin(cameraMoveAmount);
}

void tiltClockwise() {
  upPos.x = upPos.x * cos(-cameraMoveAmount) + rightDir.x * sin(-cameraMoveAmount);
  upPos.y = upPos.y * cos(-cameraMoveAmount) + rightDir.y * sin(-cameraMoveAmount);
  upPos.z = upPos.z * cos(-cameraMoveAmount) + rightDir.z * sin(-cameraMoveAmount);

  rightDir.x = rightDir.x * cos(-cameraMoveAmount) - upPos.x * sin(-cameraMoveAmount);
  rightDir.y = rightDir.y * cos(-cameraMoveAmount) - upPos.y * sin(-cameraMoveAmount);
  rightDir.z = rightDir.z * cos(-cameraMoveAmount) - upPos.z * sin(-cameraMoveAmount);
}

void translateTowardsSphere() {
  if (baseTriangleScale > 0.0) {
    // make the triangles smaller
    baseTriangleScale -= 1.0 / CONVERSION_STEPS;
    baseTriangleCenterX += baseTriangleCentroidX / CONVERSION_STEPS;
    baseTriangleCenterY += baseTriangleCentroidY / CONVERSION_STEPS;
    baseTriangleCenterZ += baseTriangleCentroidZ / CONVERSION_STEPS;

    // make the sphere faces bigger
    sphereFaceScale += 1.0 / CONVERSION_STEPS;
    sphereTranslationX -= 1.0 / CONVERSION_STEPS;

    // reduce cylinder height and translate it
    cylinderHeight -= maxCylinderHeight / CONVERSION_STEPS;
    cylinderTranslationX -= cylinderMaxTranslationX / CONVERSION_STEPS;
  }
}

void translateTowardsOctahedron() {
  if (baseTriangleScale < 1.0) {
    // make the triangles bigger
    baseTriangleScale += 1.0 / CONVERSION_STEPS;
    baseTriangleCenterX -= baseTriangleCentroidX / CONVERSION_STEPS;
    baseTriangleCenterY -= baseTriangleCentroidY / CONVERSION_STEPS;
    baseTriangleCenterZ -= baseTriangleCentroidZ / CONVERSION_STEPS;

    // make the sphere faces smaller
    sphereFaceScale -= 1.0 / CONVERSION_STEPS;
    sphereTranslationX += 1.0 / CONVERSION_STEPS;

    // increase cylinder height and translate it
    cylinderHeight += maxCylinderHeight / CONVERSION_STEPS;
    cylinderTranslationX += cylinderMaxTranslationX / CONVERSION_STEPS;
  }
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
      rotationAngle += rotationAmount;
      break;
    case 'd':
      rotationAngle -= rotationAmount;
      break;
    case 'w':
      eyePos.y += cameraMoveAmount;
      break;
    case 's':
      eyePos.y -= cameraMoveAmount;
      break;
    case ',':
      translateTowardsSphere();
      break;
    case '.':
      translateTowardsOctahedron();
      break;

    default:
      return;
  }
  glutPostRedisplay(); // Post a paint request to activate display()
}

void moveForward() {
  eyePos.x += relativeLookingPoint.x;
  eyePos.y += relativeLookingPoint.y;
  eyePos.z += relativeLookingPoint.z;
}

void moveBackward() {
  eyePos.x -= relativeLookingPoint.x;
  eyePos.y -= relativeLookingPoint.y;
  eyePos.z -= relativeLookingPoint.z;
}

void moveLeft() {
  eyePos.x += rightDir.x;
  eyePos.y += rightDir.y;
  eyePos.z += rightDir.z;
}

void moveRight() {
  eyePos.x -= rightDir.x;
  eyePos.y -= rightDir.y;
  eyePos.z -= rightDir.z;
}

void moveUp() {
  eyePos.x += upPos.x;
  eyePos.y += upPos.y;
  eyePos.z += upPos.z;
}

void moveDown() {
  eyePos.x -= upPos.x;
  eyePos.y -= upPos.y;
  eyePos.z -= upPos.z;
}

void specialKeyListener(int key, int x, int y) {
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

void drawAxes()
{
  glLineWidth(3);
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
}

void drawBaseTriangle() {
  glBegin(GL_TRIANGLES);
  glVertex3f(1, 0, 0);
  glVertex3f(0, 1, 0);
  glVertex3f(0, 0, 1);
  glEnd();
}

void drawOctahedronHead() {
  GLdouble angle = 90;

  for (int i = 0; i < 4; i++)
  {
    if (i % 2 == 0)
      glColor3d(0.302, 0.753, 0.71);
    else
      glColor3d(0.965, 0.427, 0.608);

    glRotated(angle, 0, 1, 0);

    glPushMatrix();
    glTranslated(baseTriangleCenterX, baseTriangleCenterY, baseTriangleCenterZ);
    glScaled(baseTriangleScale, baseTriangleScale, baseTriangleScale);
    drawBaseTriangle();
    glPopMatrix();
  }
}

void drawOctahedron() {
  glPushMatrix();
  drawOctahedronHead();
  glPopMatrix();

  glPushMatrix();
  glRotated(180, 0, 0, 1);
  drawOctahedronHead();
  glPopMatrix(); 
}

void drawSphereFace(int slices) {
  const float DEG2RAD = acos(-1) / 180.0f;

  float n1[3]; // normal of longitudinal plane rotating along Y-axis
  float n2[3]; // normal of latitudinal plane rotating along Z-axis
  float v[3];  // direction vector intersecting 2 planes, n1 x n2
  float a1;    // longitudinal angle along Y-axis
  float a2;    // latitudinal angle along Z-axis

  // compute the number of vertices per row, 2^n + 1
  int pointsPerRow = (int)pow(2, slices) + 1;
  // std::cout << "pointsPerRow: " << pointsPerRow << std::endl;

  struct point points[pointsPerRow][pointsPerRow];

  // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
  for (unsigned int i = 0; i < pointsPerRow; ++i)
  {
    // normal for latitudinal plane
    // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
    // therefore, it is rotating (0,1,0) vector by latitude angle a2
    a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
    n2[0] = -sin(a2);
    n2[1] = cos(a2);
    n2[2] = 0;

    // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
    for (unsigned int j = 0; j < pointsPerRow; ++j)
    {
      // normal for longitudinal plane
      // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
      // therefore, it is rotating (0,0,-1) vector by longitude angle a1
      a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
      n1[0] = -sin(a1);
      n1[1] = 0;
      n1[2] = -cos(a1);

      // find direction vector of intersected line, n1 x n2
      v[0] = n1[1] * n2[2] - n1[2] * n2[1];
      v[1] = n1[2] * n2[0] - n1[0] * n2[2];
      v[2] = n1[0] * n2[1] - n1[1] * n2[0];

      // normalize direction vector and then multiply by radius
      float scale = radius / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
      v[0] *= scale;
      v[1] *= scale;
      v[2] *= scale;

      points[i][j].x = v[0];
      points[i][j].y = v[1];
      points[i][j].z = v[2];
    }
  }

  glBegin(GL_QUADS);
  for (int j = 0; j < pointsPerRow-1; j++)
  {
    for (int i = 0; i < pointsPerRow-1; i++)
    {
      glVertex3f(points[j][i].x, points[j][i].y, points[j][i].z);
      glVertex3f(points[j][i + 1].x, points[j][i + 1].y, points[j][i + 1].z);

      glVertex3f(points[j + 1][i + 1].x, points[j + 1][i + 1].y, points[j + 1][i + 1].z);
      glVertex3f(points[j + 1][i].x, points[j + 1][i].y, points[j + 1][i].z);
    }
  }
  glEnd();
}

void drawSphereFaceWithTranslation() {
  glPushMatrix();
  glTranslated(sphereTranslationX, sphereTranslationY, sphereTranslationZ);
  glScaled(sphereFaceScale, sphereFaceScale, sphereFaceScale);
  drawSphereFace(5);
  glPopMatrix();
}

void drawSphere(double radius)
{ 
  GLfloat angle = 90;

  glPushMatrix();

  // side faces
  for (int i = 0; i < 4; i++) {
    if (i % 2)
      glColor3d(0.204, 0.565, 0.863);
    else
      glColor3d(0.22, 0.757, 0.447);

    drawSphereFaceWithTranslation();
    glRotated(angle, 0, 1, 0);
  }

  glPopMatrix();

  glPushMatrix();

  // glColor3d(0.89, 0.204, 0.184);
  glColor3d(0.584, 0.38, 0.886);
  glRotated(angle, 0, 0, 1);      // upper face
  drawSphereFaceWithTranslation();

  glRotated(angle * 2, 0, 0, 1);
  drawSphereFaceWithTranslation(); // lower face
  glPopMatrix();
}

void drawCylinder(int slices) {
  GLfloat tempx = radius, tempy = 0;
  GLfloat currx, curry;
  GLfloat theta = -cylinderTheta / 2.0;
  GLfloat halfHeight = cylinderHeight / 2.0;

  glPushMatrix();

  glBegin(GL_QUADS);
  for (int i = 1; i <= slices; i++)
  {
    theta += cylinderTheta / slices;
    currx = radius * cos(theta);
    curry = radius * sin(theta);

    glVertex3f(currx, curry, halfHeight);
    glVertex3f(currx, curry, -halfHeight);

    glVertex3f(tempx, tempy, -halfHeight);
    glVertex3f(tempx, tempy, halfHeight);

    tempx = currx;
    tempy = curry;
  }
  glEnd();
  glPopMatrix();
}

void drawCylinderFace() {
  glPushMatrix();

  glRotated(45, 0, 1, 0);
  glTranslated(cylinderTranslationX, 0, 0);
  glScaled(sphereFaceScale, sphereFaceScale, 1);
  drawCylinder(100);

  glPopMatrix();
}

void drawOneCylinderSet() {
  glPushMatrix();

  for (int i = 0; i < 4; i++)
  {
    drawCylinderFace();
    glRotated(90, 0, 1, 0);
  }

  glPopMatrix();
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
  glLoadIdentity();           // Reset the model-view matrix

  // control viewing (or camera)
  gluLookAt(eyePos.x, eyePos.y, eyePos.z,
            eyePos.x + relativeLookingPoint.x, eyePos.y + relativeLookingPoint.y, eyePos.z + relativeLookingPoint.z,
            upPos.x, upPos.y, upPos.z);

  if (isAxes)
    drawAxes();

  glScaled(2, 2, 2);
  glRotated(rotationAngle, 0, 1, 0);

  // draw the octahedron
  if (isOctahedron) {
    glPushMatrix();
    drawOctahedron();
    glPopMatrix();
  }

  // draw the sphere
  if (isSphere) {
    glPushMatrix();
    drawSphere(0.577);
    glPopMatrix();
  }

  // draw the cylinder
  glPushMatrix();
  glColor3d(0.965, 0.6, 0.247);
  
  drawOneCylinderSet();

  glRotated(90, 1, 0, 0);
  drawOneCylinderSet();

  glRotated(90, 0, 0, 1);
  drawOneCylinderSet();
  glPopMatrix();

  glutSwapBuffers(); // Render now
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char **argv)
{
  eyePos.x = 0;
  eyePos.y = 0;
  eyePos.z = 8;

  relativeLookingPoint.x = 0;
  relativeLookingPoint.y = 0;
  relativeLookingPoint.z = -1;
  upPos.x = 0;
  upPos.y = 1;
  upPos.z = 0;
  rightDir.x = 1;
  rightDir.y = 0;
  rightDir.z = 0;

  glutInit(&argc, argv);                                    // Initialize GLUT
  glutInitWindowSize(640, 640);                             // Set the window's initial width & height
  glutInitWindowPosition(50, 50);                           // Position the window's initial top-left corner
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color
  glutCreateWindow("Magic Cube");                           // Create a window with the given title
  glutDisplayFunc(display);                                 // Register display callback handler for window re-paint
  glutReshapeFunc(reshapeListener);                         // Register callback handler for window re-shape
  glutKeyboardFunc(keyboardListener);                       // Register callback handler for normal-key event
  glutSpecialFunc(specialKeyListener);                      // Register callback handler for special-key event
  initGL();                                                 // Our own OpenGL initialization
  glutMainLoop();                                           // Enter the event-processing loop
  return 0;
}