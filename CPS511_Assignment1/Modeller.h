#ifndef MODELLER
#define MODELLER

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <list>
#include "Vector3.h"
#include "CubeMesh.h"
#include "QuadMesh.h"
#include "GLApp.h"

class Modeller: public GLApp
{
public:
  // Cube Mesh Array variables and initialization
  std::vector<CubeMesh*> cubes;
  std::vector<CubeMesh*> selectedCubes;
  CubeMesh* selector;

  Modeller();
  void setUpScene() override;
  void display() override;
  void mouse(int button, int state, int x, int y) override;
  void mouseMotionHandler(int xMouse, int yMouse) override;
  void keyboard(unsigned char key, int x, int y) override;
  void functionKeys(int key, int x, int y) override;
  void translate(Vector3 diff);
  void rotate(Vector3 diff);
  void extrude(Vector3 diff);
  void raise(Vector3 diff);
  void selectCube(CubeMesh* cube);
  void select(Vector3 diff);
  void scale(Vector3 diff);

  enum Action { TRANSLATE, ROTATE, SCALE, EXTRUDE, RAISE, SELECT, MULTIPLESELECT };
  enum Action currentAction = TRANSLATE;

  QuadMesh *floorMesh = nullptr;
  QuadMesh *wallMeshes[3];
  BBox *roomBox;
  int meshSize = 16;
  std::vector<Line*> lines;
};
#endif