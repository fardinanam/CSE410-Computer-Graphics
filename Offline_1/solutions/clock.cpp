#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <ctime>

#define g 9.8f

// Global variables
#define INIT_WINDOW_WIDTH 640
#define INIT_WINDOW_HEIGHT 640
#define INIT_WINDOW_POS_X 700
#define INTI_WINDOW_POS_Y 200

#define MAX_SEC 60
#define MAX_MIN 60
#define MAX_HOUR 12
#define PENDULUM_LENGTH 0.5f
#define PENDULUM_THETA_MAX 35.0f
#define PENDULUM_TIME_INTERVAL 10


GLfloat angleSec, angleMin, angleHour;
GLfloat pendulumTheta = PENDULUM_THETA_MAX;
GLfloat w = M_PI; // T = 2pi*sqrt(l/g) = 2 => l = 1 => w = 2pi / (2pi * sqrt(1/g))

int pendulum_t = 0;

void drawPoint(GLfloat x, GLfloat y) {
  glBegin(GL_POINTS);
  glVertex2d(x, y);
  glEnd();
}

void drawCircle(GLfloat cx, GLfloat cy, GLfloat radius, int segments) {
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < segments; i++) {
    float theta = 2.0f * M_PI * (GLfloat)i / (GLfloat)segments; // get the current angle
    GLfloat x = radius * cosf(theta); // calculate the x component
    GLfloat y = radius * sinf(theta); // calculate the y component
    glVertex2f(x + cx, y + cy); // output vertex
  }
  glEnd();
}

void drawCircleFilled(GLfloat cx, GLfloat cy, GLfloat radius, int segments) {
  GLdouble twicePi = 2.0 * M_PI;

  glBegin(GL_POLYGON);  // BEGIN CIRCLE
  glVertex2f(cx, cy);        // center of circle
  for (int i = 0; i <= segments; i++) {
    double theta = i * twicePi / segments;
    
    glVertex2f(cx + ((radius) * cosf(theta)), 
              cy + ((radius) * sinf(theta)));
  }
  glEnd();
}

void drawDividers(GLfloat cx, GLfloat cy, GLfloat length, GLfloat radius, int n) {
  // glBegin(GL_LINES);
  for (int i = 0; i < n; i++) {
    float theta = 2.0f * M_PI * (GLfloat)i / (GLfloat)n;

    GLfloat x1 = radius * cosf(theta);
    GLfloat y1 = radius * sinf(theta);

    drawCircleFilled(x1 + cx, y1 + cy, length, 30);
  }
  // glEnd();
}

void drawClockOutline() {
  glBegin(GL_POLYGON);
  // glColor3d(0.929, 0.973, 0.796);
  glVertex2d(-0.45, 0.9);
  glVertex2d(-0.5, 0.85);
  // glColor3d(1, 1, 1);
  glVertex2d(-0.5, -0.5);
  // glColor3d(0.929, 0.973, 0.796);
  glVertex2d(0, -0.9);
  // glColor3d(1, 1, 1);
  glVertex2d(0.5, -0.5);
  // glColor3d(0.929, 0.973, 0.796);
  glVertex2d(0.5, 0.85);
  glVertex2d(0.45, 0.9);
  glEnd();
}

void drawHand(GLfloat width, GLfloat length, GLfloat angle) {
  GLfloat quadLength = length * 0.7;
  GLfloat x = width / 2.0;
  angle = -angle;

  glPushMatrix();

  glRotated(angle, 0, 0, 1);
  glBegin(GL_QUADS);
  glVertex2d(x, 0);
  glVertex2d(x, quadLength);
  glVertex2d(-x, quadLength);
  glVertex2d(-x, 0);
  glEnd();

  drawCircleFilled(0, quadLength, x, 20);

  glPopMatrix();
}

void drawRectangle(GLfloat width, GLfloat height) {
  GLfloat x = width / 2;

  glBegin(GL_QUADS);
  glVertex2d(x, 0);
  glVertex2d(x, -height);
  glVertex2d(-x, -height);
  glVertex2d(-x, 0);
  glEnd();
}

void drawPendulum() {
  GLfloat upperCircleRadius = 0.035;
  GLfloat shaftLength = PENDULUM_LENGTH;
  GLfloat shaftWidth = 0.04;
  GLfloat lowerCircleRadius = 0.05;

  drawCircleFilled(0, 0, upperCircleRadius, 15); // upper circle
  drawRectangle(shaftWidth, shaftLength);           // shaft
  drawCircleFilled(0, -upperCircleRadius - shaftLength, lowerCircleRadius, 25);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glColor3d(0.929, 0.973, 0.796);
  glPushMatrix();
  glTranslated(0, -0.05, 0);
  glScaled(0.9, 0.9, 1);
  drawClockOutline();
  glPopMatrix();

  glPushMatrix();
  glTranslated(0, 0.3, 0);

  glColor3d(0, 0.878, 0.439);
  drawCircleFilled(0, 0, 0.4, 100); // outer circle
  glColor3d(0, 0.192, 0.267);
  drawCircleFilled(0, 0, 0.3, 50); // inner circle

  glColor3d(1, 1, 1);
  drawDividers(0, 0, 0.005, 0.28, 60); // small dividers
  glLineWidth(3);
  drawDividers(0, 0, 0.01, 0.28, 12); // large dividers
  glLineWidth(1);

  // hour hand
  glColor3d(0, 0.878, 0.439);
  drawHand(0.034, 0.18, angleHour);
  // minute hand
  glColor3d(1, 1, 1);
  drawHand(0.02, 0.24, angleMin);
  // second hand
  drawHand(0.01, 0.3, angleSec);
  glPushMatrix();
  glRotated(180, 0, 0, 1 );
  drawHand(0.01, 0.07, angleSec);
  glPopMatrix();

  glPointSize(10);
  drawCircleFilled(0, 0, 0.017, 30); // center point
  glPopMatrix();

  glPushMatrix();
  glTranslated(0, -0.05, 0);
  glColor3d(0, 0.192, 0.267);
  glRotated(pendulumTheta, 0, 0, 1);
  drawPendulum();

  glPopMatrix();

  glFlush();
}

void clockTimer(int value) {
  glutTimerFunc(1000, clockTimer, 0);

  std::time_t t = std::time(0);  // get time now
  std::tm* now = std::localtime(&t);

  GLfloat sec = now->tm_sec;
  GLfloat min = now->tm_min + sec / MAX_SEC;
  GLfloat hour = now->tm_hour + min / MAX_MIN;

  // std::cout << now->tm_hour << ':' << now->tm_min << ":"
  //           << now->tm_sec << std::endl;
  
  angleSec = (360.0 / MAX_SEC) * sec;
  angleMin = (360.0 / MAX_MIN) * min;
  angleHour = (360.0 / MAX_HOUR) * hour;

  glutPostRedisplay();
}

void pendulumTimer(int value) {
  glutTimerFunc(PENDULUM_TIME_INTERVAL, pendulumTimer, 0);

  pendulum_t = (pendulum_t + PENDULUM_TIME_INTERVAL) % 2000;
  pendulumTheta = PENDULUM_THETA_MAX * cosf(w * pendulum_t / 1000.0);

  glutPostRedisplay();
}

void reshape(int width, int height) {
  if (height == 0) height = 1;
  GLfloat aspect = (GLfloat)width / height;

  // set the viewport to cover the new window
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (width >= height)
    gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1, 1);
  else
    gluOrtho2D(-1, 1, -1.0 / aspect, 1.0 / aspect);
}

void init() { 
  glClearColor(0, 0, 0, 1); 
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
  glutInitWindowPosition(INIT_WINDOW_POS_X, INTI_WINDOW_POS_Y);
  glutCreateWindow("Clock");
  
  glutDisplayFunc(display);
  glutTimerFunc(0, clockTimer, 0);
  glutTimerFunc(0, pendulumTimer, 0);

  glutReshapeFunc(reshape);
  init();
  glutMainLoop();

  return 0;
}