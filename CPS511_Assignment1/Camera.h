#ifndef CAMERA
#define CAMERA
#include <windows.h>
#include <gl/glu.h>
#include <gl/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "GLUtils.h"

class CubeMesh;
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
  CubeMesh* parent;
  Camera::Camera(GLApp* gl_app);
  void Follow(CubeMesh* c);
 void display();
  void perspective() const;
  Ray ScreenToWorldRay(int x, int y) const;
};
#endif