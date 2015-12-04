#include "Player.h"
#include "Camera.h"
#include "Game.h"
# define M_PIl          3.141592653589793238462643383279502884L /* pi */

//=******************************************************************
//Scene Modelling Program
//=********************************************************************
Player::Player(Game * game)
{
	this->game = game;
	this->camera = this->game->mainCamera;
	mouseSensitivity = 1.0f;
	moveSpeed = 0.1f;
	forwardDir = Vector3(0, 0, 0);
}

void Player::turnPlayer(int xMouseDiff)
{
	rotation.y += (xMouseDiff) * mouseSensitivity;
	updateCamera();
	//xMousePrev = xMouse;
	
}
void Player::updateCamera()
{
	camera->pos = center;
	float ang = rotation.y * DEG2RAD - M_PIl / 2;
	forwardDir = Vector3(-cos(ang), 0, sin(ang));
	camera->target = camera->pos + forwardDir;
}
void Player::movePlayer(int x, int y)
{
	if (x == 0 && y == 0) return;
	Vector3 rightDir = forwardDir.CrossProduct(camera->up);
	//printf("before: %f %f %f\n", center.x, center.y, center.z);
	
	translate((y * forwardDir + x * rightDir) * moveSpeed, game->roomBox);
	//printf("after : %f %f %f\n", center.x, center.y, center.z);
	updateCamera();
}

