#include "Robot.h"
#include "Game.h"
#include <iostream>

Robot::Robot(Game*g) :CubeMesh(Textures::PROFESSOR)
{
	this->game = g;
	moveSpeed = 0.01f;
  health = 1;
  setScale(Vector3(1.5f, 1.5f, 1.5f));
}
void Robot::translate(Vector3 diff)
{
  CubeMesh::translate(diff);
  Vector3 p = getWorldPos();
  p.y = -2.0f;
  //translate(Vector3(0, -1.0f, 0));
}
void Robot::setVelocity(Vector3 dir)
{
	velocity = dir * moveSpeed;
}
void Robot::setRandDirection()
{
	int randAngle = rand() % 360;
	setVelocity(Vector3(1, 0, 0).GetRotatedY(randAngle));
  setRotation(Vector3(0, -randAngle+90.f, 0));
}
void Robot::update(int deltaTime)
{
	Vector3 v = velocity * float(deltaTime);
  translate(v);
	if (!checkCollision())
	{
		translate(-v);
		setRandDirection();
	}
  
  if (shotCooldown)
  {
    shotTimer += deltaTime;
    if (shotTimer >= shotTimerMax)
    {
      shotTimer = 0;
      shotCooldown = false;
}
  }
  Room * playerRoom = Game::instance->roomAt(Game::instance->player->getWorldPos());
  Room * room = Game::instance->roomAt(getWorldPos());
  if (playerRoom == room)
  {
    Vector3 dir = Game::instance->player->getWorldPos() - getWorldPos();
    float ang = atan2f(dir.z, dir.x);
    ang *= (1.0f / DEG2RAD);
    setRotation(Vector3(0, -ang + 90, 0));
    if (!shotCooldown) {
      spawnBullet();
      shotCooldown = true;
    }
  }
}
bool Robot::checkCollision(bool pointBased)
{
	for (auto& bullet : Game::instance->bullets) {
		BBox other = bullet->getBBox();
    if (!bullet->shotByPlayer) continue;
		if (other.Intersects(getBBox()) && !(pointBased && !other.Contains(getWorldPos()))) {
      bullet->flaggedForRemoval = true;
			if (--health <= 0)
			{
        if (!flaggedForRemoval)
        {
          Game::instance->kills++;
        }
        flaggedForRemoval = true;
				return true;
			}
			break;
		}
	}

	for (auto& robot : Game::instance->robots) {
		BBox other = robot->getBBox();
		if (robot != this && other.Intersects(getBBox()) && !(pointBased && !other.Contains(getWorldPos()))) {
			return false;
		}
	}
	return CubeMesh::checkCollision(pointBased);
}
void Robot::spawnBullet()
{
	Bullet * bullet = new Bullet();
  bullet->shotByPlayer = false;
  bullet->moveSpeed /= 3.0f;
	bullet->setPosition(getWorldPos());
  Vector3 forwardDir = Game::instance->player->getWorldPos() - getWorldPos();
  forwardDir.Normalize();
	bullet->setVelocity(forwardDir);
	game->bullets.push_back(bullet);
}
void Robot::drawSelf() const
{
  Game::instance->mesh->draw();
}