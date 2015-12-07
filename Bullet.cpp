#include "Bullet.h"

Bullet::Bullet():CubeMesh((Textures)0)
{
	float r = radius * 2;
  setScale(Vector3(r, r, r));
}

void Bullet::setVelocity(Vector3 dir)
{
	velocity = dir * moveSpeed;
}

void Bullet::update(int deltaTime)
{
  lifeLeft -= deltaTime;
	if (lifeLeft <= 0)
	{
		//disappear bullet
    flaggedForRemoval = true;
	}
	translate(velocity * float(deltaTime));
  checkCollision(true);
}

void Bullet::drawSelf() const
{
  glDisable(GL_TEXTURE_2D);
	Material* matptr = selected ? &highlightMaterial : &material;
  if (shotByPlayer)
    matptr->ambient = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
  else
    matptr->ambient = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	matptr->glApply();
  //glColor3f(1.0f, 0.0f, 0.0f);
	GLUquadricObj *sphereObj = gluNewQuadric();
	gluSphere(sphereObj, 1.0, 10, 10);
  glEnable(GL_TEXTURE_2D);
}
bool Bullet::checkCollision(bool pointBased)
{
  if (!CubeMesh::checkCollision(pointBased))
  {
    flaggedForRemoval = true;
    return false;
  }
  return true;
}