#include "Transform.h"


void Transform::draw() const {

  glPushMatrix();
  /*glTranslatef(position.x, position.y, position.z);
  glRotatef(rotation.x, 1, 0, 0);
  glRotatef(rotation.y, 0, 1, 0);
  glRotatef(rotation.z, 0, 0, 1);
  glScalef(scale.x, scale.y, scale.z);*/
  glMultMatrixf(currentMatrix.get());

  drawSelf();
  drawChildren();

  glPopMatrix();
}

void Transform::updateMatrix() {
  currentMatrix = (parent == nullptr) ? Matrix4() : parent->currentMatrix;
  currentMatrix.scale(scale);
  currentMatrix.rotateX(rotation.x);
  currentMatrix.rotateY(rotation.y);
  currentMatrix.rotateZ(rotation.z);
  currentMatrix.translate(position);
  for (auto& child : children) child->updateMatrix();
}

void Transform::drawSelf() const {}

void Transform::drawChildren() const {
  for(auto& child : children) {
    child->draw();
  }
}

void Transform::translate(Vector3 diff)
{
  setPosition(position + diff);

}
void Transform::Scale(Vector3 diff)
{
  setScale(scale.ElementWiseProduct(diff));
}
void Transform::rotateEulers(Vector3 rot)
{
  Vector3 res = Vector3();
  res.x = float(fmod(rotation.x + rot.x, 360));
  res.y = float(fmod(rotation.y + rot.y, 360));
  res.z = float(fmod(rotation.z + rot.z, 360));
  setRotation(res);
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

Vector3 Transform::getPosition() const {
  return position;
}

void Transform::setPosition(Vector3 position) {
  this->position = position;
  updateMatrix();
}

Vector3 Transform::getScale() const {
  return scale;
}

void Transform::setScale(Vector3 scale) {
  this->scale = scale;
  updateMatrix();

}

Vector3 Transform::getRotation() const {
  return rotation;
}

void Transform::setRotation(Vector3 rotation) {
  this->rotation = rotation;
  updateMatrix();

}

Transform::~Transform()
{
}
