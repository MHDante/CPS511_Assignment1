#pragma once
#include "GLUtils.h"
#include <set>

class Transform
{
private:
  Transform* parent;

  Vector3 position = Vector3(0, 0, 0);
  Vector3 scale = Vector3(1, 1, 1);
  Vector3 rotation = Vector3(0, 0, 0);

public:
  Vector3 getPosition() const {
    return position;
  }

  void setPosition(Vector3 position) {
    this->position = position;
  }

  Vector3 getScale() const {
    return scale;
  }

  void setScale(Vector3 scale) {
    this->scale = scale;
  }

  Vector3 getRotation() const {
    return rotation;
  }

  void setRotation(Vector3 rotation) {
    this->rotation = rotation;
  }
protected:
  virtual void drawSelf() const;
  virtual void drawChildren() const;
public:
  Transform* getParent() const{ return parent;}
  virtual void draw() const;
  void translate(Vector3 diff);
  void Scale(Vector3 diff);
  void rotateEulers(Vector3 rot);
  bool setParent(Transform* p);
  bool addChild(Transform* p);
  std::set<Transform*> children;
  Transform();
  explicit Transform(Transform* parent);
  virtual ~Transform();
};

