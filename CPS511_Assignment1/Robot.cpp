#include "Robot.h"
#include "Game.h"
#include <iostream>

Robot::Robot(Game*g) :CubeMesh(Textures::PROFESSOR)
{
	this->game = g;
	moveSpeed = 0.01f;
  health = 1;
  botTransform = Transform();
  //varMesh = new VarMesh("megaman.obj");
  setScale(Vector3(1.5f, 1.5f, 1.5f));
  botTransform.setScale(Vector3(0.3f, 0.3f, 0.3f));
  //setPosition(Vector3(getPosition().x, -1.0f, getPosition().z));

	/*
	auto dante = false;
	if (dante) {
		moveSpeed /= 4;
	}
	*/

}
void Robot::translate(Vector3 diff)
{
  CubeMesh::translate(diff);
  Vector3 p = getPosition();
  p.y = -2.0f;
  botTransform.setPosition(p);
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
  botTransform.setRotation(Vector3(0, -randAngle+90, 0));
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
  Room * playerRoom = Game::instance->roomAt(Game::instance->player->getPosition());
  Room * room = Game::instance->roomAt(getPosition());
  if (playerRoom == room && !shotCooldown)
  {
    spawnBullet();
    shotCooldown = true;
  }
}
bool Robot::checkCollision(bool pointBased)
{
	for (auto& bullet : Game::instance->bullets) {
		BBox other = bullet->getBBox();
    if (!bullet->shotByPlayer) continue;
		if (other.Intersects(getBBox()) && !(pointBased && !other.Contains(getPosition()))) {
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
		if (robot != this && other.Intersects(getBBox()) && !(pointBased && !other.Contains(getPosition()))) {
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
	bullet->setPosition(getPosition());
  Vector3 forwardDir = Game::instance->player->getPosition() - getPosition();
  forwardDir.Normalize();
	bullet->setVelocity(forwardDir);
	game->bullets.push_back(bullet);
}
//void Robot::drawSelf() const {
//	//CubeMesh::draw();
//  //varMesh->Draw();
//  Game::instance->mesh->Draw();
//}

void Robot::draw() const
{
  //CubeMesh::draw();

  glPushMatrix();
  glMultMatrixf(botTransform.currentMatrix.get());
  Game::instance->mesh->Draw();
  glPopMatrix();
}