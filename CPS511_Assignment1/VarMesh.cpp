#include "VarMesh.h"
#include "Game.h"
#pragma warning(disable: 4996)

void VarMesh::Draw(Material* mat) const {
  (mat == nullptr ? material : *mat).glApply();


  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glBindTexture(GL_TEXTURE_2D, Game::instance->textureMap[Textures::MEGAMAN]);

  glVertexPointer(3, GL_FLOAT, sizeof(Vector3), vertices.data());
  glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 2, uvs.data());
  glNormalPointer(GL_FLOAT, sizeof(Vector3), uvs.data());
  //glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, vertexIndices.data());
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
VarMesh::VarMesh(const char* path) {
  material = Material(
    Vector4(0, 0, 0, 1),
    Vector4(0, 0, 0, 1),
    Vector4(.9f, .5f, .0f, 1),
    0.0);
  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;

  std::vector<Vector3> temp_vertices;
  std::vector<Vector2> temp_uvs;
  std::vector<Vector3> temp_normals;

  FILE * file = fopen(path, "r");
  if (file == nullptr) {
      printf("Impossible to open the file !\n");
      throw ERROR;
  }
  int line = 0;
  while (1) {

    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
      break; // EOF = End Of File. Quit the loop.
    if (strcmp(lineHeader, "v") == 0) {
     Vector3 vertex;
     fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
     temp_vertices.push_back(vertex);
    }
    else if (strcmp(lineHeader, "vt") == 0) {
      Vector2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      temp_uvs.push_back(uv);
    }
    else if (strcmp(lineHeader, "vn") == 0) {
      Vector3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      temp_normals.push_back(normal);
    }
    else if (strcmp(lineHeader, "f") == 0) {
     std::string vertex1, vertex2, vertex3;
     unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
     int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
     if (matches != 9) {
       printf("File can't be read by our simple parser : ( Try exporting with other options\n");
       throw ERROR;

     }

       vertexIndices.push_back(vertexIndex[0]);
       vertexIndices.push_back(vertexIndex[1]);
       vertexIndices.push_back(vertexIndex[2]);
       uvIndices.push_back(uvIndex[0]);
       uvIndices.push_back(uvIndex[1]);
       uvIndices.push_back(uvIndex[2]);
       normalIndices.push_back(normalIndex[0]);
       normalIndices.push_back(normalIndex[1]);
       normalIndices.push_back(normalIndex[2]);
     }
    line++;
  }

  for (unsigned int i = 0; i < vertexIndices.size(); i++) {
    unsigned int vertexIndex = vertexIndices[i];
    Vector3 vertex = temp_vertices[vertexIndex - 1];
    vertices.push_back(vertex);
  }
  for (unsigned int i = 0; i < uvIndices.size(); i++) {
    unsigned int uvIndex = uvIndices[i];
    Vector2 uv = temp_uvs[uvIndex - 1];
    uvs.push_back(uv);
  }
  for (unsigned int i = 0; i < normalIndices.size(); i++) {
    unsigned int normalIndex = normalIndices[i];
    Vector3 normal = temp_normals[normalIndex - 1];
    normals.push_back(normal);
  }
}
