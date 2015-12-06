#ifndef __ROOM
#define __ROOM
#include "GLUtils.h"
#include "QuadMesh.h"
#include <unordered_set>

class Room : public BBox {
public:
  using BBox::Contains;
  enum Direction { LEFT, FORWARD, RIGHT, BACK };
  int meshSize = 16;
  QuadMesh *floorMesh = nullptr;
  QuadMesh *wallMeshes[4];
  Room* rooms[4] = { nullptr,nullptr,nullptr,nullptr };
  Room(Vector3 min, Vector3 max);
  ~Room() {}

  virtual bool Contains(BBox* box) const override;
  float width() const;
  float depth() const;
  float height() const;
  Vector3 center() const;
  Room* SpawnRoom(Direction dir);
  void Draw() const;

};
#endif

