
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
  int health = 0, maxHealth = 10;
  explicit Player(Game * game);
	void turnPlayer(int xMouseDiff);
	void update(int deltaTime) override;
	void movePlayer(float x, float y, int deltaTime);
	void spawnBullet();
  void draw() const override;
  bool checkCollision(bool pointBased = false) override;
private:
	int xMousePrev = 0;
};

#endif