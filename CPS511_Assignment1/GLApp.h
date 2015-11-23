#ifndef GLAPP
#define GLAPP
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include "GLUtils.h"

class Camera;

class GLApp {
public:

  GLApp();
  void init(int argc, char** argv);
  virtual void display(void) = 0;
  virtual void mouse(int button, int state, int x, int y) = 0;
  virtual void mouseMotionHandler(int xMouse, int yMouse) = 0;
  virtual void keyboard(unsigned char key, int x, int y) = 0;
  virtual void functionKeys(int key, int x, int y) = 0;
  virtual void UpdateConsole() = 0;
  virtual void setUpScene() = 0;

  void reshape(int w, int h) const;
  int currentButton;
  unsigned char currentKey;
  GLfloat light_position0[4] =  { -6.0f,  12.0f, 0.0f,1.0f };
  GLfloat light_position1[4] =  { 6.0f,  12.0f, 0.0f,1.0f };
  GLfloat light_diffuse[4] =    { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat light_specular[4] =   { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat light_ambient[4] =    { 0.2f, 0.2f, 0.2f, 1.0f };
  int WinID;// = 0;
  int ScreenWidth;
  int ScreenHeight;
  Camera* mainCamera = nullptr;
  int ScreenPosX;
  int ScreenPosY;
  const char* windowName;
};


#endif