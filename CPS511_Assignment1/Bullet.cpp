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
	if (lifeLeft-- <= 0)
	{
		//disappear bullet
    flaggedForRemoval = true;
	}
	translate(velocity * float(deltaTime));
}

void Bullet::drawSelf() const
{
	Material* matptr = selected ? &highlightMaterial : &material;
	matptr->glApply();
	GLUquadricObj *sphereObj = gluNewQuadric();
	gluSphere(sphereObj, 1.0, 10, 10);
}
