#pragma once
#include <vector>
#include "GLUtils.h"
#include "Transform.h"

class VarMesh {
public:
  Transform transform;
  Material material;
  VarMesh();

public:
  void Draw(Material* mat = nullptr) const;
  bool loadOBJ(const char * path);
  std::vector<Vector3> vertices;
  std::vector<Vector2> uvs;
  std::vector<Vector3> normals;

};
