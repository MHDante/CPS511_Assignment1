#pragma once
#include <vector>
#include "GLUtils.h"
#include "Transform.h"

class VarMesh : public Transform {
public:
  Material material;
  void drawSelf() const override;
  explicit VarMesh(const char * path);

public:
  std::vector<Vector3> vertices;
  std::vector<Vector2> uvs;
  std::vector<Vector3> normals;

};
