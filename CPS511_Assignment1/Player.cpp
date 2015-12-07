#include "Player.h"
#include "Camera.h"
#include "Game.h"
#include <iostream>

Player::Player(Game*g):CubeMesh((Textures)0)
{
	this->game = g;
  mouseSensitivity = .1f;
  moveSpeed = 0.02f;
  health = maxHealth;
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
  if (Game::instance->lostGame) return;
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
  for (auto& bullet : Game::instance->bullets) {
    BBox other = bullet->getBBox();
    if (bullet->shotByPlayer) continue;
    if (getBBox().Contains(bullet->getWorldPos())) {//if (other.Intersects(getBBox())){// && !(pointBased && !other.Contains(getWorldPos()))) {
      bullet->flaggedForRemoval = true;
      if (--health <= 0)
      {
        Game::instance->lostGame = true;
        //game over
        //return true;
      }
    }
  }
}

void Player::spawnBullet()
{
  if (Game::instance->lostGame) return;
	Bullet * bullet = new Bullet();
  bullet->shotByPlayer = true;
	bullet->setPosition(getWorldPos());
	Vector3 forwardDir = Vector3(0, 0, -1).GetRotatedY(-getRotation().y);
	bullet->setVelocity(forwardDir);
	game->bullets.push_back(bullet);
}
void Player::draw() const {
  
}
bool Player::checkCollision(bool pointBased)
{
  return CubeMesh::checkCollision(pointBased);
}