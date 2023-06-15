#include <GL/glut.h>
#include <iostream>
#include <cmath>

#define CONVERSION_STEPS 16.0f

// Global variables
GLfloat eyex = 4, eyey = 4, eyez = 4;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;

const GLfloat baseTriangleCentroidX = 0.3333, baseTriangleCentroidY = 0.3333, baseTriangleCentroidZ = 0.3333; 

GLfloat baseTriangleCenterX = 0, baseTriangleCenterY = 0, baseTriangleCenterZ = 0;
GLfloat baseTriangleScale = 1.0;

GLfloat sphereFaceScale = 0;
GLfloat sphereTranslationX = 1.0, sphereTranslationY = 0.0, sphereTranslationZ = 0.0;


GLfloat PHI = acos(-1 / 3), // angle between two faces of the octahedron
        radius = 0.577;     // radius of the sphere

bool isAxes = true, isOctahedron = true, isSphere = true;

struct point {
  GLfloat x, y, z;
};

/* Initialize OpenGL Graphics */
void initGL()
{
  // Set "clearing" or background color
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
  glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}

void reshapeListener(GLsizei width, GLsizei height)
{ // GLsizei for non-negative integer
  // Compute aspect ratio of the new window
  if (height == 0)
    height = 1; // To prevent divide by 0
  GLfloat aspect = (GLfloat)width / (GLfloat)height;

  // Set the viewport to cover the new window
  glViewport(0, 0, width, height);

  // Set the aspect ratio of the clipping area to match the viewport
  glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
  glLoadIdentity();            // Reset the projection matrix
  /*if (width >= height) {
      // aspect >= 1, set the height from -1 to 1, with larger width
      gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
  } else {
      // aspect < 1, set the width to -1 to 1, with larger height
      gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
  }*/
  // Enable perspective projection with fovy, aspect, zNear and zFar
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void keyboardListener(unsigned char key, int x, int y)
{
  double v = 0.25;
  double lx = centerx - eyex;
  double lz = centerz - eyez;
  double s;
  switch (key)
  {
  case 'a':
    eyex += v * (upy * lz);
    eyez += v * (-lx * upy);
    s = sqrt(eyex * eyex + eyez * eyez) / (4 * sqrt(2));
    eyex /= s;
    eyez /= s;
    break;
  case 'd':
    eyex += v * (-upy * lz);
    eyez += v * (lx * upy);
    s = sqrt(eyex * eyex + eyez * eyez) / (4 * sqrt(2));
    eyex /= s;
    eyez /= s;
    break;
  case 'w':
    eyey += v;
    break;
  case 's':
    eyey -= v;
    break;
  case ',':
    if (baseTriangleScale) {
      // make the triangles smaller
      baseTriangleScale -= 1.0 / CONVERSION_STEPS;
      baseTriangleCenterX += baseTriangleCentroidX / CONVERSION_STEPS;
      baseTriangleCenterY += baseTriangleCentroidY / CONVERSION_STEPS;
      baseTriangleCenterZ += baseTriangleCentroidZ / CONVERSION_STEPS;

      // make the sphere faces bigger
      sphereFaceScale += 1.0 / CONVERSION_STEPS;
      sphereTranslationX -= 1.0 / CONVERSION_STEPS;
    }
    break;
  case '.':
    if (baseTriangleScale < 1.0) {
      // make the triangles bigger
      baseTriangleScale += 1.0 / CONVERSION_STEPS;
      baseTriangleCenterX -= baseTriangleCentroidX / CONVERSION_STEPS;
      baseTriangleCenterY -= baseTriangleCentroidY / CONVERSION_STEPS;
      baseTriangleCenterZ -= baseTriangleCentroidZ / CONVERSION_STEPS;

      // make the sphere faces smaller
      sphereFaceScale -= 1.0 / CONVERSION_STEPS;
      sphereTranslationX += 1.0 / CONVERSION_STEPS;
    }
    break;
  case 'o':
    isOctahedron = !isOctahedron;
    break;
  case 'p':
    isSphere = !isSphere;
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
  // GLfloat phiMin = 54.74 * M_PI / 180, phiMax = 125.26 * M_PI / 180;
  // GLfloat phiMin = -PHI / 2, longitudeArcLength = M_PI - PHI;
  // GLfloat thetaMin = M_PI / 4, latitudeArcLength = M_PI / 2;

  // struct point points[slices + 1][slices + 1];

  // for (int i = 0; i <= slices; i++) {
  //   GLfloat phi = phiMin + i * longitudeArcLength / slices;
  //   GLfloat y = r * sin(phi);
    
  //   for (int j = 0; j <= slices; j++) {
  //     GLfloat theta = thetaMin + j * latitudeArcLength / slices;
  //     GLfloat x = r * cos(phi) * sin(theta);
  //     GLfloat z = r * cos(phi) * cos(theta);

  //     points[i][j].x = x;
  //     points[i][j].y = y;
  //     points[i][j].z = z;
  //   }
  // }

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

      // std::cout << "drawing: (" << points[j][i].x << ',' << points[j][i].y << ',' << points[j][i].z << ") ";
      // std::cout << "(" << points[j][i + 1].x << ',' << points[j][i + 1].y << ',' << points[j][i + 1].z << ") ";
      // std::cout << "(" << points[j + 1][i + 1].x << ',' << points[j + 1][i + 1].y << ',' << points[j + 1][i + 1].z << ") ";
      // std::cout << "(" << points[j + 1][i].x << ',' << points[j + 1][i].y << ',' << points[j + 1][i].z << ") ";
      // std::cout << std::endl;
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

void display()
{
  // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
  glLoadIdentity();           // Reset the model-view matrix

  // control viewing (or camera)
  gluLookAt(eyex, eyey, eyez,
            centerx, centery, centerz,
            upx, upy, upz);

  if (isAxes)
    drawAxes();

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
  glutSwapBuffers(); // Render now
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char **argv)
{
  glutInit(&argc, argv);                                    // Initialize GLUT
  glutInitWindowSize(640, 640);                             // Set the window's initial width & height
  glutInitWindowPosition(50, 50);                           // Position the window's initial top-left corner
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color
  glutCreateWindow("Magic Cube");                           // Create a window with the given title
  glutDisplayFunc(display);                                 // Register display callback handler for window re-paint
  glutReshapeFunc(reshapeListener);                         // Register callback handler for window re-shape
  glutKeyboardFunc(keyboardListener);                       // Register callback handler for normal-key event
  // glutSpecialFunc(specialKeyListener);                      // Register callback handler for special-key event
  initGL();                                                 // Our own OpenGL initialization
  glutMainLoop();                                           // Enter the event-processing loop
  return 0;
}