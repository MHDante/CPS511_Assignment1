
#ifndef PLAYER
#define PLAYER

#include "CubeMesh.h"
#include "Bullet.h"

class Game;

class Player : public CubeMesh
{
public:
	Game * game;
	float mouseSensitivity, moveSpeed;
  explicit Player(Game * game);
	void turnPlayer(int xMouseDiff);
	void update(int deltaTime) override;
	void movePlayer(float x, float y, int deltaTime);
	void spawnBullet();
  void draw() const override;
private:
	int xMousePrev = 0;
};

#endif