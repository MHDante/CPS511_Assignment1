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
#include <map>
#include <list>
#include "GLUtils.h"
#include "CubeMesh.h"
#include "GLApp.h"
#include "Player.h"
#include "RGBpixmap.h"

#include "Room.h"
#include "VarMesh.h"
#include "Robot.h"

class Game : public GLApp
{
public:
	// Cube Mesh Array variables and initialization
  static Game* instance;
  
	std::vector<CubeMesh*> cubes;
	std::vector<Bullet*> bullets;
	std::vector<Robot*> robots;
	std::map<Textures, GLuint> textureMap;
	std::map<Textures, RGBpixmap> pixmaps;
	Player * player;
	Vector3 rightLeft, upDown;
	int centerX;
	int centerY;
	int previousTime = 0, spawnTimer = 0, spawnTimerMax = 2000, spawnTimerMaxRand = 0;
  int kills = 0;
  VarMesh* mesh;
	Game();
	void setUpScene() override;
	void display() override;
	void mouse(int button, int state, int x, int y) override;
	void mouseMotionHandler(int xMouse, int yMouse) override;
	void keyboard(unsigned char key, int x, int y) override;
	void functionKeys(int key, int x, int y) override;
	void keyboardRelease(unsigned char key, int x, int y) override;
	void UpdateConsole() override;
	void idleFunc() override;
	void spawnEnemy();
	void loadTextures();
	void loadTexture(const char * filename, Textures tex);

	void recenterMouse();
	Room* roomAt(Vector3 center);

  std::vector<Room*> rooms;
	std::vector<Line> lines;
};


#endif
