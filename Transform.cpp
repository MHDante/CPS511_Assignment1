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

  glPopMatrix();
  drawChildren();

}

void Transform::updateMatrix() {
  auto parentMatrix = (parent == nullptr) ? Matrix4() : parent->currentMatrix;
  currentMatrix = Matrix4();
  currentMatrix.scale(scale);
  currentMatrix.rotateX(rotation.x);
  currentMatrix.rotateY(rotation.y);
  currentMatrix.rotateZ(rotation.z);
  currentMatrix.translate(position);

  currentMatrix = parentMatrix * currentMatrix;
  for (auto& child : children) child->updateMatrix();
  updateGeometry();


  inverseMatrix = Matrix4(currentMatrix);
  inverseMatrix.invert();



}

void Transform::updateGeometry() {}

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

Vector3 Transform::getWorldPos() const {
  if (parent == nullptr) return position;
  return parent->currentMatrix * position;
}

Vector3 Transform::getLocalPos() const {
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


// Given a cube mesh, compute it's current bounding box and return in vectors min and max
// i.e. compute min.x,min.y,mi.z,max.x,max.y,max.z
// Use this function for collision detection of cube and walls/floor
BBox Transform::getBBox() const
{
  //could be optimized by memoization

  Vector3 min = Vector3(-.5f, -.5f, -.5f);
  Vector3 max = Vector3(.5f, .5f, .5f);

  Vector3 edgePoints[8] = {
    currentMatrix * (min),
    currentMatrix * (min + Vector3(FORWARD)),
    currentMatrix * (min + Vector3(RIGHT)),
    currentMatrix * (min + Vector3(UP)),
    currentMatrix * (max),
    currentMatrix * (max + Vector3(BACK)),
    currentMatrix * (max + Vector3(LEFT)),
    currentMatrix * (max + Vector3(DOWN)) };

  max.x = -INFINITY;
  max.y = -INFINITY;
  max.z = -INFINITY;
  min.x = INFINITY;
  min.y = INFINITY;
  min.z = INFINITY;

  for (auto pt : edgePoints) {
    if (pt.x > max.x) max.x = pt.x;
    if (pt.y > max.y) max.y = pt.y;
    if (pt.z > max.z) max.z = pt.z;
    if (pt.x < min.x) min.x = pt.x;
    if (pt.y < min.y) min.y = pt.y;
    if (pt.z < min.z) min.z = pt.z;
  }
  auto b = BBox(min, max);
  return b;
}
