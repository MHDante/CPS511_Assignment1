#pragma once
#include "GLUtils.h"
#include <set>

class Transform
{

public:
  Vector3 position, scale, rotation;
  Transform* parent;
  std::set<Transform*> children;
  Transform();
  ~Transform();
};

