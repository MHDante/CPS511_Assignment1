#ifndef CUBEMESH
#define CUBEMESH
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <utility>
#include <vector>
#include "GLUtils.h"
#include "QuadMesh.h"




// Vertex positions of a standard size cube (width 2), centered at the origin
// of its own Model Coordinate System

class CubeMesh
{

  QuadMesh faces[6] = {
    QuadMesh(1, Vector3(-1, 1, -1), Vector3(0, 0, 2), Vector3(2, 0, 0)),
    QuadMesh(1, Vector3(1, -1, 1), Vector3(-2, 0, 0), Vector3(0, 0, -2)),
    QuadMesh(1, Vector3(-1, -1, -1), Vector3(0, 0, 2), Vector3(0, 2, 0)),
    QuadMesh(1, Vector3(1, 1, 1), Vector3(0, -2, 0), Vector3(0, 0, -2)),
    QuadMesh(1, Vector3(1, 1, 1), Vector3(-2, 0, 0), Vector3(0, -2, 0)),
    QuadMesh(1, Vector3(-1, -1, -1), Vector3(0, 2, 0), Vector3(2, 0, 0))
  };

public:
	Vector3 center;
	Vector3 dim;
	int angle;				// Angle around y-axis of cube coordinate system
	bool selected;
  bool hovered;
  static Material material; // Material properties for drawing
  static Material highlightMaterial;	// Material properties if selected

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
  Vector3 Intersects(Ray* ray) const;
  static bool singleSelecting;
};
#endif