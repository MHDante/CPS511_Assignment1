#include "Camera.h"
#include "GLApp.h"

Camera::Camera(GLApp* gl_app) {
    glApp = gl_app;
    fovY = 60;
    aspect = glApp->ScreenWidth / glApp->ScreenHeight;
    nearZ = 0.3;
    farZ = 100;
  }

  void Camera::display() const
  {
    gluLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z, up.x, up.y, up.z);
  }
  void Camera::perspective() const
  {
    gluPerspective(fovY, aspect, nearZ, farZ);
  }

  Vector3 Camera::ScreenToWorld(int x, int y) const
  {
    Vector3 eyeDir = Vector3(x / (glApp->ScreenWidth / 2.0) - 1, y / (glApp->ScreenHeight / 2.0) - 1, 1 / tan(0.523599)) * 22;
    auto camTarget = Vector3() - pos;
    auto angle = -atan2(camTarget.y, camTarget.z);
    auto dir2 = Vector3(
      eyeDir.x,
      eyeDir.y * cos(angle) - eyeDir.z* sin(angle),
      eyeDir.y* sin(angle) + eyeDir.z * cos(angle));
    return dir2;
  }
