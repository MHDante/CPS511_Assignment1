#include "QuadMesh.h"
#include "Game.h"


QuadMesh::QuadMesh(int meshSize, Vector3 origin, Vector3 length, Vector3 width, Textures texture)
	: origin(origin), length(length), width(width), texture(texture){

  normal = length.CrossProduct(width).Normalized();
  numVertices = (meshSize + 1) * (meshSize + 1);
  numQuads = (meshSize)* (meshSize);
  vertices = new Vector3[numVertices];
  texcoords = new GLfloat[numVertices * 2];
  quads = new GLuint[numQuads * 4];

  // Setup the material and lights used for the mesh
  material = Material(
    Vector4(0, 0, 0, 1),
    Vector4(0, 0, 0, 1),
    Vector4(.9f, .5f, .0f, 1),
    0.0);

  length /= float(meshSize);
  width /= float(meshSize);

  int currentVertex = 0;
  for (int i = 0; i < meshSize + 1; i++) {
    for (int j = 0; j < meshSize + 1; j++) {
    vertices[currentVertex] = origin + length * float(j);
	  texcoords[currentVertex * 2] = float(i) / meshSize;
	  texcoords[currentVertex * 2 + 1] = float(j) / meshSize;
	  currentVertex++;
    }
    origin += width; // go to next row in mesh (negative z direction)
  }

  // Build Quad Polygons
  int currentQuad = 0;
  for (int j = 0; j < meshSize; j++) {
    for (int k = 0; k < meshSize; k++) {
      // Counterclockwise order
      quads[currentQuad++] = j * (meshSize + 1) + k;
      quads[currentQuad++] = j * (meshSize + 1) + k + 1;
      quads[currentQuad++] = (j + 1) * (meshSize + 1) + k + 1;
      quads[currentQuad++] = (j + 1) * (meshSize + 1) + k;
    }
  }

}

QuadMesh::~QuadMesh() {
  if (vertices) delete[] vertices;
  if (quads) delete[] quads;
}

void QuadMesh::DrawMesh(Material* mat) const {
  (mat == nullptr?material:*mat).glApply();
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glBindTexture(GL_TEXTURE_2D, Game::instance->textureMap[texture]);

  glNormal3f(normal.x, normal.y, normal.z);
  glVertexPointer(3, GL_FLOAT, sizeof(Vector3), vertices);
  glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 2, texcoords);
  glDrawElements(GL_QUADS, numQuads * 4, GL_UNSIGNED_INT, quads);


  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

Vector3 QuadMesh::intersectsRay(Ray r) const {
  //from wikipedia https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
  float bot = r.dir.DotProduct(normal);
  if (bot == 0) return Vector3::Sentinel();
  Vector3 p0 = origin;
  float top = (p0 - r.origin).DotProduct(normal);
  float d = top / bot;
  Vector3 intersectionPt = (r.origin + d * r.dir);

  Vector3 localPt = intersectionPt - origin;

  float projW = width.DotProduct(localPt) / width.GetLength();
  if (projW < 0 || projW > width.GetLength()) return Vector3::Sentinel();

  float projH = length.DotProduct(localPt) / length.GetLength();
  if (projH < 0 || projH > length.GetLength()) return Vector3::Sentinel();
  
  return intersectionPt;

}
