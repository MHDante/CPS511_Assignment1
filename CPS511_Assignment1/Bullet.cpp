
#include "Bullet.h"
#include "Game.h"
# define M_PIl          3.141592653589793238462643383279502884L

Bullet::Bullet(Room* r) :CubeMesh(r){};

void Bullet::setVelocity(Vector3 dir)
{
	velocity = dir * moveSpeed;
}

void Bullet::update()
{
	if (lifeLeft-- <= 0)
	{
		//disappear bullet
	}
	translate(velocity);
}

void Bullet::display()
{
	glPushMatrix();
	//glPushAttrib(GL_LIGHTING_BIT);
	glTranslatef(center.x, center.y, center.z);
	//glRotatef(rotation.y, 0, 1, 0);
	glScalef(dim.x / 2, dim.y / 2, dim.z / 2);
	Material* matptr = selected ? &highlightMaterial : &material;
	matptr->glApply();
	GLUquadricObj *sphereObj = gluNewQuadric();
	gluSphere(sphereObj, 0.2f, 10, 10);
	glPopMatrix();
	//glPopAttrib();
}
