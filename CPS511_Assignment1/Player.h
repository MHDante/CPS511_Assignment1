
#ifndef PLAYER
#define PLAYER

#include "CubeMesh.h"
#include "Camera.h"

class Game;

class Player : public CubeMesh
{
public:
	float mouseSensitivity, moveSpeed;
	Game * game;
	Camera * camera;
	Player(Game * game);
	void turnPlayer(int xMouseDiff);
	void updateCamera();
	void movePlayer(int x, int y);
private:
	int xMousePrev = 0;
	Vector3 forwardDir;
};

#endif