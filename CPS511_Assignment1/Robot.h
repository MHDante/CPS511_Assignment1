#ifndef ROBOT
#define ROBOT

#include "CubeMesh.h"
#include "Bullet.h"

class Game;

class Robot : public CubeMesh
{
public:
	Game * game;
	float moveSpeed;
	Vector3 velocity;
	int health;
	explicit Robot(Game * game);
	void setVelocity(Vector3 dir);
	void setRandDirection();
	void update(int deltaTime) override;
	bool checkCollision(bool pointBased = false) override;
	void spawnBullet();
	void draw() const override;
};

#endif