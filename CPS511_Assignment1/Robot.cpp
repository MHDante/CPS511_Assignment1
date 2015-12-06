#include "Robot.h"
#include "Game.h"
#include <iostream>

Robot::Robot(Game*g) :CubeMesh(Textures::BOT)
{
	this->game = g;
	moveSpeed = 0.01f;
	/*
	auto dante = false;
	if (dante) {
		moveSpeed /= 4;
	}
	*/

}
void Robot::setVelocity(Vector3 dir)
{
	velocity = dir * moveSpeed;
}
void Robot::setRandDirection()
{
	int randAngle = rand() % 360;
	setVelocity(Vector3(1, 0, 0).GetRotatedY(randAngle));
}
void Robot::update(int deltaTime)
{
	Vector3 v = velocity * deltaTime;
	if (!translate(v))
	{
		translate(-v);
		setRandDirection();
	}
}
bool Robot::checkCollision(bool pointBased)
{
	for (auto& bullet : Game::instance->bullets) {
		BBox other = bullet->getBBox();
		if (other.Intersects(getBBox()) && !(pointBased && !other.Contains(center))) {
			printf("HIT");
			///AAARRGGGG
			Game::instance->bullets.erase(std::remove(Game::instance->bullets.begin(), Game::instance->bullets.end(), bullet), Game::instance->bullets.end());
			
			delete(bullet); 
			if (--health <= 0)
			{
				Game::instance->robots.erase(std::remove(Game::instance->robots.begin(), Game::instance->robots.end(), this), Game::instance->robots.end());
				delete(this);
				return true;
			}
			break;
			//return false;
		}
	}

	for (auto& robot : Game::instance->robots) {
		BBox other = robot->getBBox();
		if (robot != this && other.Intersects(getBBox()) && !(pointBased && !other.Contains(center))) {
			return false;
		}
	}

	return CubeMesh::checkCollision(pointBased);


}
void Robot::spawnBullet()
{
	Bullet * bullet = new Bullet();
	bullet->center = center;
	Vector3 forwardDir = Vector3(0, 0, -1).GetRotatedY(-rotation.y);
	bullet->setVelocity(forwardDir);
	game->bullets.push_back(bullet);
	//printf("%d", game->bullets.size());
}
void Robot::draw() const {
	CubeMesh::draw();
}