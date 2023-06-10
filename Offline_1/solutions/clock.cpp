#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <ctime>

#define pi 3.1415926f
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
#define PENDULUM_THETA_MAX 45.0f
#define PENDULUM_TIME_INTERVAL 100

#define w g/PENDULUM_LENGTH

GLfloat angleSec, angleMin, angleHour;
GLfloat pendulumTheta = PENDULUM_THETA_MAX;
int pendulum_t = 0;

void drawPoint(GLfloat x, GLfloat y) {
  glBegin(GL_POINTS);
  glVertex2d(x, y);
  glEnd();
}

void drawCircle(GLfloat cx, GLfloat cy, GLfloat radius, int segments) {
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < segments; i++) {
    float theta = 2.0f * pi * (GLfloat)i / (GLfloat)segments; // get the current angle
    GLfloat x = radius * cosf(theta); // calculate the x component
    GLfloat y = radius * sinf(theta); // calculate the y component
    glVertex2f(x + cx, y + cy); // output vertex
  }
  glEnd();
}

void drawCircledFilled(GLfloat cx, GLfloat cy, GLfloat radius, int segments) {
  GLdouble twicePi = 2.0 * pi;

  glBegin(GL_TRIANGLE_FAN);  // BEGIN CIRCLE
  glVertex2f(cx, cy);        // center of circle
  for (int i = 0; i <= segments; i++) {
    double theta = i * twicePi / segments;
    
    glVertex2f(cx + ((radius) * cosf(theta)), 
              cy + ((radius) * sinf(theta)));
  }
  glEnd();
}

void drawDividers(GLfloat cx, GLfloat cy, GLfloat length, GLfloat radius, int n) {
  glBegin(GL_LINES);
  for (int i = 0; i < n; i++) {
    float theta = 2.0f * pi * (GLfloat)i / (GLfloat)n;

    GLfloat x1 = radius * cosf(theta);
    GLfloat y1 = radius * sinf(theta);
    GLfloat x2 = (radius - length) * cosf(theta);
    GLfloat y2 = (radius - length) * sinf(theta);

    glVertex2d(x1 + cx, y1 + cy);
    glVertex2d(x2 + cx, y2 + cy);
  }
  glEnd();
}

void drawClockOutline() {
  glBegin(GL_LINE_LOOP);
  glVertex2d(-0.5, 0.9);
  glVertex2d(-0.5, -0.5);
  glVertex2d(0, -0.9);
  glVertex2d(0.5, -0.5);
  glVertex2d(0.5, 0.9);
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

  glBegin(GL_TRIANGLES);
  glVertex2d(x, quadLength);
  glVertex2d(0, length);
  glVertex2d(-x, quadLength);
  glEnd();

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

  drawCircledFilled(0, 0, upperCircleRadius, 15); // upper circle
  drawRectangle(shaftWidth, shaftLength);           // shaft
  drawCircledFilled(0, -upperCircleRadius - shaftLength, lowerCircleRadius, 25);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  drawClockOutline();

  glPushMatrix();
  glTranslated(0, 0.3, 0);
  drawCircle(0, 0, 0.3, 40); // inner circle
  drawCircle(0, 0, 0.4, 50); // outer circle

  drawDividers(0, 0, 0.02, 0.3, 60); // small dividers
  drawDividers(0, 0, 0.03, 0.3, 12); // large dividers

  glPointSize(10);
  drawPoint(0, 0); // center point

  drawHand(0.01, 0.27, angleSec); // second hand
  drawHand(0.015, 0.2, angleMin); // minute hand
  drawHand(0.02, 0.15, angleHour); // hour hand
  glPopMatrix();

  glPushMatrix();

  glTranslated(0, -0.05, 0);
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

  pendulumTheta = PENDULUM_THETA_MAX * cosf(w * pendulum_t);
  pendulum_t = pendulum_t + PENDULUM_TIME_INTERVAL;
  if(pendulum_t == 2001) pendulum_t = 1;

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

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
  glutInitWindowPosition(INIT_WINDOW_POS_X, INTI_WINDOW_POS_Y);
  glutCreateWindow("Clock");
  
  glutDisplayFunc(display);
  glutTimerFunc(0, clockTimer, 0);
  glutTimerFunc(0, pendulumTimer, 0);

  glutReshapeFunc(reshape);
  glutMainLoop();

  return 0;
}