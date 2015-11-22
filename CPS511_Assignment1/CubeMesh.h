#ifndef CUBEMESH
#define CUBEMESH
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <utility>
#include <vector>
#include "Vector3.h"

typedef struct BoundingBox
{
  Vector3 min;
  Vector3 max;
} BBox;


// Vertex positions of a standard size cube (width 2), centered at the origin
// of its own Model Coordinate System

class CubeMesh
{
  const Vector3 vertices[8] = {
     Vector3(-1.0, -1.0,-1.0 ),
     Vector3(1.0, -1.0,-1.0  ),
     Vector3(1.0,  1.0,-1.0  ),
     Vector3(-1.0,  1.0,-1.0 ),
     Vector3(-1.0, -1.0, 1.0 ),
     Vector3(1.0, -1.0, 1.0  ),
     Vector3(1.0,  1.0, 1.0  ),
     Vector3(-1.0,  1.0, 1.0 ) };

  const GLubyte quads[4*6] = {
    0, 3, 2, 1,	  // back face
    2, 3, 7, 6,	  // top face
    0, 4, 7, 3,   // left face
    1, 2, 6, 5,   // right face
    4, 5, 6, 7,   // front face
    0, 1, 5, 4 };	// bottom face

  const Vector3 quadNormals[6] = {
     Vector3(0.0, 0.0, -1.0 ),	// Back Face 
     Vector3(0.0, 1.0,  0.0 ),	// Top Face
     Vector3(-1.0, 0.0,  0.0),	// Left Face
     Vector3(1.0, 0.0,  0.0 ),	// Right Face
     Vector3(0.0, 0.0,  1.0 ),	// Front Face
     Vector3(0.0,-1.0,  0.0 )};	// Bottom Face

public:
	Vector3 center;
	Vector3 dim;
	int angle;				// Angle around y-axis of cube coordinate system
	bool selected;
  bool hovered;

	// Material properties for drawing
	static Vector4 mat_ambient;
  static Vector4 mat_specular;
  static Vector4 mat_diffuse;
	static float mat_shininess;
	// Material properties if selected
  static Vector4 highlightMat_ambient;
  static Vector4 highlightMat_specular;
  static Vector4 highlightMat_diffuse;
	static float highlightMat_shininess;

  //Methods

  CubeMesh();
  bool isWithin(BBox* container) const;
  void drawSelector() const;
  BBox getBBox() const;
  void drawCube() const;
  bool translate(Vector3 diff, BBox* bounds);
  bool scale(Vector3 diff, BBox* bounds);
  bool rotate(Vector3 diff, BBox* bounds);
  bool extrude(Vector3 diff, BBox* bounds);
  bool raise(Vector3 diff, BBox* bounds);
  static bool singleSelecting;
};
#endif