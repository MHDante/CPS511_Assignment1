#include "Transform.h"


void Transform::draw() const {

  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glRotatef(rotation.x, 1, 0, 0);
  glRotatef(rotation.y, 0, 1, 0);
  glRotatef(rotation.z, 0, 0, 1);
  glScalef(scale.x, scale.y, scale.z);

  drawSelf();
  drawChildren();

  glPopMatrix();
}

void Transform::drawSelf() const {}

void Transform::drawChildren() const {
  for(auto& child : children) {
    child->draw();
  }
}

void Transform::translate(Vector3 diff)
{
  position += diff;
}
void Transform::Scale(Vector3 diff)
{
  scale = scale.ElementWiseProduct(diff);
}
void Transform::rotateEulers(Vector3 rot)
{
  rotation.x = float(fmod(rotation.x + rot.x, 360));
  rotation.y = float(fmod(rotation.y + rot.y, 360));
  rotation.z = float(fmod(rotation.z + rot.z, 360));
}

bool Transform::setParent(Transform* p)
{
  Transform* t = parent;
  while (t!=nullptr)
  {
    if (t == this) return false;
    t = t->getParent();
  }
  if(parent!= nullptr) parent->children.erase(this);

  parent = p;
  parent->children.insert(this);
  return true;
}
bool Transform::addChild(Transform* p)
{
  return p->setParent(this);
}
Transform::Transform()
{
  parent = nullptr;
}

Transform::Transform(Transform* parent)
{
  setParent(parent);
}

Transform::~Transform()
{
}
