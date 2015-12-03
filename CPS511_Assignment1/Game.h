#ifndef GAME
#define GAME

#include <windows.h>
#include "QuadMesh.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <list>
#include "GLUtils.h"
#include "CubeMesh.h"
#include "GLApp.h"

class Game : public GLApp
{
public:
	// Cube Mesh Array variables and initialization
	std::vector<CubeMesh*> cubes;
	std::vector<CubeMesh*> selectedCubes;
	CubeMesh* selector;

	Game();
	void setUpScene() override;
	void display() override;
	void mouse(int button, int state, int x, int y) override;
	void mouseMotionHandler(int xMouse, int yMouse) override;
	void keyboard(unsigned char key, int x, int y) override;
	void functionKeys(int key, int x, int y) override;
	void UpdateConsole() override;

	QuadMesh *floorMesh = nullptr;
	QuadMesh *wallMeshes[3];
	BBox *roomBox;
	int meshSize = 16;
	std::vector<Line> lines;
};
#endif
