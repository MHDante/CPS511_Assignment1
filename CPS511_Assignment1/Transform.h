#pragma once
#include "GLUtils.h"
#include <set>

class Transform
{
private:
  Transform* parent;
public:
  Transform* getParent() const{ return parent;}
  bool setParent(Transform* p);
  bool addChild(Transform* p);
  Vector3 position = Vector3(0, 0, 0);
  Vector3 scale = Vector3(1, 1, 1);
  Vector3 rotation = Vector3(0, 0, 0);
  std::set<Transform*> children;
  Transform();
  explicit Transform(Transform* parent);
  virtual ~Transform();
};

