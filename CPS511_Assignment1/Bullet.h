
#ifndef BULLET
#define BULLET

#include "CubeMesh.h"


class Game;

class Bullet : public CubeMesh
{
public:
	float moveSpeed = 2;
	Vector3 velocity;
	explicit Bullet(Room* r);
	void setVelocity(Vector3 dir);
	void update();
	void display();
};

#endif
