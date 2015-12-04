#include "Player.h"
#include "Camera.h"
#include "Game.h"
#include <iostream>

//=******************************************************************
//Scene Modelling Program
//=********************************************************************
Player::Player(Game*g,Room*r):CubeMesh(r)
{
	this->game = g;
	mouseSensitivity = .1f;
	moveSpeed = 0.6f;
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
  Vector3 dir = (y * forwardDir + x * rightDir) * moveSpeed;
  if (!translate(dir)) translate(-dir);
	//printf("after : %f %f %f\n", center.x, center.y, center.z);
}
void Player::update()
{
}

void Player::spawnBullet()
{
	Bullet * bullet = new Bullet(room);
	bullet->center = center;
	Vector3 forwardDir = Vector3(0, 0, -1).GetRotatedY(-rotation.y);
	bullet->setVelocity(forwardDir);
	game->bullets.push_back(bullet);
	//printf("%d", game->bullets.size());
}
void Player::drawCube() const {
  
}