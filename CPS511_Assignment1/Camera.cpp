#include "Camera.h"
#include "GLApp.h"
#include <iostream>

Camera::Camera(GLApp* gl_app) {
    glApp = gl_app;
    fovY = 60;
    aspect = glApp->ScreenWidth / glApp->ScreenHeight;
    nearZ = 0.3;
    farZ = 100;
    target = Vector3(0,0,0);
    pos = Vector3(0,0,10);
    up = Vector3(0, 1, 0);
  }

  void Camera::display() const
  {
    gluLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z, up.x, up.y, up.z);
  }
  void Camera::perspective() const
  {
    gluPerspective(fovY, aspect, nearZ, farZ);
  }

  Vector3 Camera::ScreenToWorldDir(int x, int y) const
  {
    auto deg2Rad = (M_PI / 180);
    y = glApp->ScreenHeight -y; //Opengl...
    float xRatio = (glApp->ScreenWidth / 2.0);
    float yRatio = (glApp->ScreenHeight / 2.0);
    float zRatio = tan(fovY * deg2Rad);
    Vector3 eyeDir = Vector3(x / xRatio, y/yRatio, zRatio);
    eyeDir.x *= aspect;
    eyeDir -= Vector3(1, 1, 0);
    eyeDir.Normalize();

    auto fwdVector = (target - pos);
    auto rightVector = fwdVector.CrossProduct(up);
    auto upVector = rightVector.CrossProduct(fwdVector);

    fwdVector.Normalize();
    rightVector.Normalize();
    upVector.Normalize();
    

    auto dir2 = Vector3(
      eyeDir.x*rightVector.x + eyeDir.y*upVector.x + eyeDir.z*fwdVector.x,
      eyeDir.x*rightVector.y + eyeDir.y*upVector.y + eyeDir.z*fwdVector.y,
      eyeDir.x*rightVector.z + eyeDir.y*upVector.z + eyeDir.z*fwdVector.z
      );

    auto vector3 = dir2;
    std::cout << vector3.toString() << std::endl;
    return vector3;
  }
