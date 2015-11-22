#include "QuadMesh.h"

QuadMesh::QuadMesh(int maxMeshSize, float meshDim)
{
  minMeshSize = 1;
  numVertices = 0;
  vertices = nullptr;
  numQuads = 0;
  quads = nullptr;

  this->maxMeshSize = maxMeshSize < minMeshSize ? minMeshSize : maxMeshSize;
  this->meshDim = meshDim;
  CreateMemory();

  // Setup the material and lights used for the mesh
  mat_ambient = Vector3(0, 0, 0);
  mat_specular = Vector3(0, 0, 0);
  mat_diffuse = Vector3(.9, .5, .0);
  mat_shininess = 0.0;

}

bool QuadMesh::CreateMemory()
{
  vertices = new Vector3[(maxMeshSize + 1)*(maxMeshSize + 1)]();
  normals = new Vector3[(maxMeshSize + 1)*(maxMeshSize + 1)]();
  quads = new GLuint[maxMeshSize*maxMeshSize * 4]();
  if (!vertices || !normals || !quads)
  {
    return false;
  }

  return true;
}



bool QuadMesh::InitMesh(int meshSize, Vector3 origin, double meshLength, double meshWidth, Vector3 dir1, Vector3 dir2)
{



  numVertices = (meshSize + 1)*(meshSize + 1);
  dir1 *= meshLength / meshSize;
  dir2 *= meshWidth / meshSize;

  int currentVertex = 0;
  for (int i = 0; i< meshSize + 1; i++){
    for (int j = 0; j< meshSize + 1; j++){
      vertices[currentVertex] = origin + dir1 * j;
      currentVertex++;
    }
    origin += dir2; // go to next row in mesh (negative z direction)
  }

  // Build Quad Polygons
  numQuads = (meshSize)*(meshSize);
  int currentQuad = 0;

  for (int j = 0; j < meshSize; j++)
  {
    for (int k = 0; k < meshSize; k++)
    {
      // Counterclockwise order
      quads[currentQuad++] = j*    (meshSize + 1) + k;
      quads[currentQuad++] = j*    (meshSize + 1) + k + 1;
      quads[currentQuad++] = (j + 1)*(meshSize + 1) + k + 1;
      quads[currentQuad++] = (j + 1)*(meshSize + 1) + k;
    }
  }

  this->ComputeNormals();
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  return true;
}

void QuadMesh::DrawMesh(int meshSize) const
{
  int currentQuad = 0;

  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient.toGLFloat4(1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular.toGLFloat4(1.0));
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse.toGLFloat4(1.0));
  glMaterialfv(GL_FRONT, GL_SHININESS, &mat_shininess);
  glNormalPointer(GL_FLOAT, sizeof(Vector3), normals);
  glVertexPointer(3, GL_FLOAT, sizeof(Vector3), vertices);
  glDrawElements(GL_QUADS, numQuads*4, GL_UNSIGNED_INT, quads);
}

void QuadMesh::FreeMemory()
{
  if (vertices)
    delete[] vertices;
  vertices = nullptr;
  numVertices = 0;

  if (quads)
    delete[] quads;
  quads = nullptr;
  numQuads = 0;
}

void QuadMesh::ComputeNormals() const
{
  int currentQuad = 0;

  for (int j = 0; j< this->maxMeshSize; j++)
  {
    for (int k = 0; k< this->maxMeshSize; k++)
    {
      Vector3 n0, n1, n2, n3, e0, e1, e2, e3, ne0, ne1, ne2, ne3;

      e0 = vertices[quads[currentQuad * 4 + 1]] - vertices[quads[currentQuad * 4 + 0]];
      e1 = vertices[quads[currentQuad * 4 + 2]] - vertices[quads[currentQuad * 4 + 1]];
      e2 = vertices[quads[currentQuad * 4 + 3]] - vertices[quads[currentQuad * 4 + 2]];
      e3 = vertices[quads[currentQuad * 4 + 0]] - vertices[quads[currentQuad * 4 + 3]];

      e0.Normalize();
      e1.Normalize();
      e2.Normalize();
      e3.Normalize();

      normals[quads[currentQuad * 4 + 0]] += e0.CrossProduct(-e3).Normalized();
      normals[quads[currentQuad * 4 + 1]] += e1.CrossProduct(-e0).Normalized();
      normals[quads[currentQuad * 4 + 2]] += e2.CrossProduct(-e1).Normalized();
      normals[quads[currentQuad * 4 + 3]] += e3.CrossProduct(-e2).Normalized();

      normals[quads[currentQuad*4+0]].Normalize();
      normals[quads[currentQuad*4+1]].Normalize();
      normals[quads[currentQuad*4+2]].Normalize();
      normals[quads[currentQuad*4+3]].Normalize();

      currentQuad++;
    }
  }
}