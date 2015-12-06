#include "Transform.h"


bool Transform::setParent(Transform* p)
{
  Transform* t = parent;
  while (t!=nullptr)
  {
    if (t == this) return false;
    t = t->getParent();
  }
  if(parent!= nullptr) parent->children.erase(this);
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
