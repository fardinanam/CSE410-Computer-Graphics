#include <GL/glut.h>
#include <cmath>

// Global variables
GLfloat eyex = 4, eyey = 4, eyez = 4;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;

GLfloat PHI = acos(-1 / 3), // angle between two faces of the octahedron
              r = 0.577;    // radius of the sphere

bool isAxes = true;

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
    drawBaseTriangle();
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
  GLfloat phiMin = -PHI / 2, longitudeArcLength = M_PI - PHI;
  GLfloat thetaMin = M_PI / 4, latitudeArcLength = M_PI / 2;

  struct point points[slices + 1][slices + 1];

  for (int i = 0; i <= slices; i++) {
    GLfloat phi = phiMin + i * longitudeArcLength / slices;
    GLfloat y = r * sin(phi);
    
    for (int j = 0; j <= slices; j++) {
      GLfloat theta = thetaMin + j * latitudeArcLength / slices;
      GLfloat x = r * cos(phi) * sin(theta);
      GLfloat z = r * cos(phi) * cos(theta);

      points[i][j].x = x;
      points[i][j].y = y;
      points[i][j].z = z;
    }
  }

  glBegin(GL_QUADS);
  for (int j = 0; j < slices; j++)
  {
    for (int i = 0; i < slices; i++)
    {
      // GLfloat c = (2 + cos((i + j) * 2.0 * M_PI / slices)) / 3;
      // glColor3f(c, c, c);
      glVertex3f(points[j][i].x, points[j][i].y, points[j][i].z);
      glVertex3f(points[j][i + 1].x, points[j][i + 1].y, points[j][i + 1].z);

      glVertex3f(points[j + 1][i + 1].x, points[j + 1][i + 1].y, points[j + 1][i + 1].z);
      glVertex3f(points[j + 1][i].x, points[j + 1][i].y, points[j + 1][i].z);
    }
  }
  glEnd();
}

void drawSphere(double radius, int stacks, int slices)
{ 
  GLfloat angle = 90;

  glPushMatrix();

  for (int i = 0; i < 4; i++) {
    if (i % 2)
      // glColor3d(0.22, 0.757, 0.447);
      glColor3d(0.204, 0.565, 0.863);
    else
      glColor3d(0.302, 0.753, 0.72);
    drawSphereFace(10);
    glRotated(angle, 0, 1, 0);
  }

  glPopMatrix();

  glPushMatrix();

  glRotated(angle, 1, 0, 0);
  glColor3d(0.204, 0.565, 0.863);
  // drawSphereFace(10);

  // glRotated(angle * 2, 0, 0, 1);
  // glColor3d(0.584, 0.38, 0.886);
  // drawSphereFace(10);
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
  // glPushMatrix();
  // drawOctahedron();
  // glPopMatrix();

  glPushMatrix();
  drawSphere(0.577, 40, 100);
  glPopMatrix();

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