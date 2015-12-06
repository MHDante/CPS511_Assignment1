#pragma once
#include <vector>
#include "GLUtils.h"
#include "Transform.h"

class VarMesh : public Transform {
public:
  Material material;
  explicit VarMesh(const char * path);

public:
  void Draw(Material* mat = nullptr) const;
  std::vector<Vector3> vertices;
  std::vector<Vector2> uvs;
  std::vector<Vector3> normals;

};
