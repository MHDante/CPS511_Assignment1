#include "Player.h"
#include "Camera.h"
#include "Game.h"
#include <iostream>

Player::Player(Game*g):CubeMesh((Textures)0)
{
	this->game = g;
  mouseSensitivity = .1f;
  moveSpeed = 0.02f;
  /*
  auto dante = false;
  if (dante) {
    moveSpeed /= 4;
  }
  */

}

void Player::turnPlayer(int xMouseDiff)
{
  auto diff = xMouseDiff * mouseSensitivity;
  rotateEulers(Vector3(0,diff,0));
}

void Player::movePlayer(float x, float y, int deltaTime)
{
	if (x == 0 && y == 0) return;
	Vector3 forwardDir = Vector3(0,0,-1).GetRotatedY(-getRotation().y);
  Vector3 rightDir = Vector3(1,0,0).GetRotatedY(-getRotation().y);
  Vector3 dir = (y * forwardDir + x * rightDir) * moveSpeed * float(deltaTime);
  translate(dir);
  if (!checkCollision(true)) translate(-dir);
	//printf("after : %f %f %f\n", position.x, position.y, position.z);
}
void Player::update(int deltaTime)
{

}

void Player::spawnBullet()
{
	Bullet * bullet = new Bullet();
	bullet->setPosition(getPosition());
	Vector3 forwardDir = Vector3(0, 0, -1).GetRotatedY(-getRotation().y);
	bullet->setVelocity(forwardDir);
	game->bullets.push_back(bullet);
	//printf("%d", game->bullets.size());
}
void Player::draw() const {
  
}