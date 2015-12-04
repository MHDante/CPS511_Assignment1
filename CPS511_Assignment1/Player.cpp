#include "Player.h"
#include "Camera.h"
#include "Game.h"
#include <iostream>

//=******************************************************************
//Scene Modelling Program
//=********************************************************************
Player::Player(Room*r):CubeMesh(r)
{
	mouseSensitivity = .1f;
	moveSpeed = 0.1f;
}

void Player::turnPlayer(int xMouseDiff)
{
  auto diff = xMouseDiff * mouseSensitivity;
  rotate(Vector3(0,diff,0));
  std::cout << diff << std::endl;
	//xMousePrev = xMouse;
	
}

void Player::movePlayer(int x, int y)
{
	if (x == 0 && y == 0) return;
	Vector3 forwardDir = Vector3(0,0,-1).GetRotatedY(-rotation.y);
  Vector3 rightDir = Vector3(1,0,0).GetRotatedY(-rotation.y);
	translate((y * forwardDir + x * rightDir) * moveSpeed);
	//printf("after : %f %f %f\n", center.x, center.y, center.z);
}

