#ifndef QUADMESH
#define QUADMESH
#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <utility>
#include <vector>
#include "Vector3.h"

class QuadMesh
{
private:

  int maxMeshSize;
  int minMeshSize;
  int numVertices;
  Vector3 *vertices;
  Vector3 *normals;
  int numQuads;
  GLuint *quads;
  
public:
  Vector3 mat_ambient;
  Vector3 mat_specular;
  Vector3 mat_diffuse;
  GLfloat mat_shininess;

  explicit QuadMesh(int maxMeshSize = 40);
  ~QuadMesh();

  bool InitMesh(int meshSize, Vector3 origin, double meshLength, double meshWidth, Vector3 dir1, Vector3 dir2);
  void DrawMesh(int meshSize) const;
  void ComputeNormals() const;
};
#endif
