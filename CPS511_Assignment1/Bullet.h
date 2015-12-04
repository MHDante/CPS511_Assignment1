
#ifndef BULLET
#define BULLET

#include "CubeMesh.h"


class Game;

class Bullet : public CubeMesh
{
public:
	float moveSpeed;
	Vector3 velocity;
	Game * game;
	Bullet(Game * game);
	void setVelocity(Vector3 dir);
	void update();
	void display();
};

#endif
