#include "Camera.h"
#include "GLApp.h"
#include <iostream>
#include "CubeMesh.h"

Camera::Camera(GLApp* gl_app) {
    glApp = gl_app;
    fovY = 60;
    aspect = float(glApp->ScreenWidth) / float(glApp->ScreenHeight);
    nearZ = 0.0f;
    farZ = 100;
    target = Vector3(0,0,0);
    pos = Vector3(0,0,10);
    up = Vector3(0, 1, 0);
  }

void Camera::Follow(CubeMesh* c) {
  parent = c;
}

void Camera::display()
  {
    if (parent != nullptr) {

      pos = parent->position;
      float ang = parent->rotation.y * DEG2RAD - M_PIl / 2;
      auto forwardDir = Vector3(-cos(ang), 0, sin(ang));
      target = pos + forwardDir;
    }
    gluLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z, up.x, up.y, up.z);
  }
  void Camera::perspective() const
  {
    gluPerspective(fovY, aspect, nearZ, farZ);
  }

  Ray Camera::ScreenToWorldRay(int x, int y) const
  {
    y = glApp->ScreenHeight -y; //Opengl...
    float xRatio = (glApp->ScreenWidth / 2.0f);
    float yRatio = (glApp->ScreenHeight / 2.0f);
    float zRatio = tan(fovY * DEG2RAD);
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

    return Ray(pos, dir2);
  }
