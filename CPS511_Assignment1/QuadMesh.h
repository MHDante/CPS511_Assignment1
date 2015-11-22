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
  float meshDim;
  int numVertices;
  Vector3 *vertices;
  Vector3 *normals;
  int numQuads;
  GLuint *quads;

  bool CreateMemory();
  void FreeMemory();

public:
  Vector3 mat_ambient;
  Vector3 mat_specular;
  Vector3 mat_diffuse;
  GLfloat mat_shininess;
  typedef std::pair<int, int> MaxMeshDim;

  QuadMesh(int maxMeshSize = 40, float meshDim = 1.0f);

  ~QuadMesh()
  {
    FreeMemory();
  }

  MaxMeshDim GetMaxMeshDimentions()
  {
    return MaxMeshDim(minMeshSize, maxMeshSize);
  }

  bool InitMesh(int meshSize, Vector3 origin, double meshLength, double meshWidth, Vector3 dir1, Vector3 dir2);
  void DrawMesh(int meshSize) const;
  void ComputeNormals() const;


};
#endif //QUADMESH
