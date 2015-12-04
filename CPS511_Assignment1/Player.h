
#ifndef PLAYER
#define PLAYER

#include "CubeMesh.h"
#include "Camera.h"

class Game;

class Player : public CubeMesh
{
public:
	float mouseSensitivity, moveSpeed;
  explicit Player(Room * room);
	void turnPlayer(int xMouseDiff);
	void movePlayer(int x, int y);
private:
	int xMousePrev = 0;
};

#endif