#include <GL/glut.h>
#include <cmath>

// Global variables
GLfloat eyex = 4, eyey = 4, eyez = 4;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;

bool isAxes = true;

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

void drawUpperCap() {
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
  drawUpperCap();
  glPopMatrix();

  glPushMatrix();
  glRotated(180, 0, 0, 1); 
  drawUpperCap();
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
  glPushMatrix();
  drawOctahedron();
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
  glutCreateWindow("OpenGL 3D Drawing");                    // Create a window with the given title
  glutDisplayFunc(display);                                 // Register display callback handler for window re-paint
  glutReshapeFunc(reshapeListener);                         // Register callback handler for window re-shape
  glutKeyboardFunc(keyboardListener);                       // Register callback handler for normal-key event
  // glutSpecialFunc(specialKeyListener);                      // Register callback handler for special-key event
  initGL();                                                 // Our own OpenGL initialization
  glutMainLoop();                                           // Enter the event-processing loop
  return 0;
}