#include "Bullet.h"

Bullet::Bullet():CubeMesh((Textures)0)
{
	float r = radius * 2;
	dim = Vector3(r, r, r);
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
	glTranslatef(center.x, center.y, center.z);
	//glRotatef(rotation.y, 0, 1, 0);
	glScalef(dim.x / 2, dim.y / 2, dim.z / 2);
	Material* matptr = selected ? &highlightMaterial : &material;
	matptr->glApply();
	GLUquadricObj *sphereObj = gluNewQuadric();
	gluSphere(sphereObj, 1.0, 10, 10);
	glPopMatrix();
	//glPopAttrib();
}
