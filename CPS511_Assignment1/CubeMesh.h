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

class Room;

class CubeMesh
{
private:
	QuadMesh *faces[6];

public:
	virtual ~CubeMesh()
	{
		delete [](faces);
	}

	Vector3 center;
	Vector3 dim;
	Vector3 rotation;				// Angle around y-axis of cube coordinate system
	bool selected;
  bool hovered;
  static Material material; // Material properties for drawing
  static Material highlightMaterial;	// Material properties if selected
  Textures texture;
  //Methods
  CubeMesh(Textures texture);
  void drawSelector() const;
  BBox getBBox() const;
  virtual void draw() const;
  virtual void update(int deltaTime);
  bool translate(Vector3 diff);
  virtual bool checkCollision(bool pointBased = false);
  bool rotateEulers(Vector3 rot);
  bool scale(Vector3 diff);
  bool rotate(Vector3 diff);
  bool extrude(Vector3 diff);
  bool raise(Vector3 diff);
  Vector3 Intersects(Ray ray) const;
  static bool singleSelecting;
};
#endif