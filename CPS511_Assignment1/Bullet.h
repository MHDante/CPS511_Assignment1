
#ifndef BULLET
#define BULLET

#include "CubeMesh.h"


class Game;

class Bullet : public CubeMesh
{
public:
	float moveSpeed = 2;
	int lifeLeft = 1000;
	Vector3 velocity;
	Bullet();
	void setVelocity(Vector3 dir);
	void update();
	void display();
};

#endif
