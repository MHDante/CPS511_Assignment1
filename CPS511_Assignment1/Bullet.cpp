#include "Bullet.h"

Bullet::Bullet():CubeMesh((Textures)0)
{
	float r = radius * 2;
	scale = Vector3(r, r, r);
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
	}
	translate(velocity * deltaTime);
}

void Bullet::draw() const
{
	glPushMatrix();
	//glPushAttrib(GL_LIGHTING_BIT);
	glTranslatef(position.x, position.y, position.z);
	//glRotatef(rotation.y, 0, 1, 0);
	glScalef(scale.x / 2, scale.y / 2, scale.z / 2);
	Material* matptr = selected ? &highlightMaterial : &material;
	matptr->glApply();
	GLUquadricObj *sphereObj = gluNewQuadric();
	gluSphere(sphereObj, 1.0, 10, 10);
	glPopMatrix();
	//glPopAttrib();
}
