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
#include "Transform.h"


// Vertex positions of a standard size cube (width 2), centered at the origin
// of its own Model Coordinate System

class Room;

class CubeMesh : public Transform
{
private:
	QuadMesh *faces[6];

public:
	virtual ~CubeMesh()
	{
    for (auto& f : faces) {
      delete f;
    }
	}

	bool selected;
  bool hovered;
  static Material material; // Material properties for drawing
  static Material highlightMaterial;	// Material properties if selected
  Textures texture;
  //Methods
  CubeMesh(Textures texture);
  void drawSelector() const;
  virtual void drawSelf() const override;
  virtual void update(int deltaTime);
  virtual bool checkCollision(bool pointBased = false);
  bool extrude(Vector3 diff);
  bool raise(Vector3 diff);
  Vector3 Intersects(Ray ray) const;
  static bool singleSelecting;
};
#endif