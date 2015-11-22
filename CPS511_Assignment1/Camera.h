#ifndef CAMERA
#define CAMERA
#include <windows.h>
#include <gl/glu.h>
#include <gl/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "Vector3.h"

class GLApp;

class Camera
{
public:
  Vector3 pos;
  Vector3 target;
  Vector3 up;
  double fovY;
  double aspect;
  double nearZ;
  double farZ;
  GLApp* glApp;
  Camera::Camera(GLApp* gl_app);
 void display() const;
  void perspective() const;
  Vector3 ScreenToWorldDir(int x, int y) const;
};
#endif