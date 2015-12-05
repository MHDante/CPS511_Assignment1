#ifndef QUADMESH
#define QUADMESH
// ReSharper disable once CppUnusedIncludeDirective
#include <windows.h>
#include <gl/gl.h>
#include "GLUtils.h"

class QuadMesh{

  Vector3 origin, length, width;
  int numVertices;
  Vector3 *vertices;
  GLfloat * texcoords;
  int numQuads;
  GLuint *quads;

public:
  Vector3 normal;
  Material material;

  explicit QuadMesh(int meshSize, Vector3 origin, Vector3 dir1, Vector3 dir2);
  ~QuadMesh();
  void DrawMesh(Material* mat = nullptr) const;
  Vector3 intersectsRay(Ray r)const;
};
#endif
