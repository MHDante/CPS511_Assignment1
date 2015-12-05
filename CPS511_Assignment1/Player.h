
#ifndef PLAYER
#define PLAYER

#include "CubeMesh.h"
#include "Camera.h"
#include "Bullet.h"

class Game;

class Player : public CubeMesh
{
public:
	Game * game;
	float mouseSensitivity, moveSpeed;
  explicit Player(Game * game);
	void turnPlayer(int xMouseDiff);
	void update();
	void movePlayer(int x, int y);
	void spawnBullet();
  void drawCube() const override;
private:
	int xMousePrev = 0;
};

#endif