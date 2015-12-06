#ifndef BULLET
#define BULLET
#include "CubeMesh.h"

class Game;

class Bullet : public CubeMesh
{
public:
	float moveSpeed = 0.05f;
	float radius = 0.2f;
	int lifeLeft = 1000;
	Vector3 velocity;
	Bullet();
	void setVelocity(Vector3 dir);
	void update(int deltaTime) override;
	void drawSelf() const override;
};

#endif
