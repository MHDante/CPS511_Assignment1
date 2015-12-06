#ifndef __ROOM
#define __ROOM
#include "GLUtils.h"
#include "QuadMesh.h"
#include <unordered_set>
#include "Transform.h"

class Room : public Transform, public BBox{
public:
  using BBox::Contains;
  BBox bounds;
  enum Direction { LEFT, FORWARD, RIGHT, BACK };
  int meshSize = 16;
  QuadMesh *floorMesh = nullptr;
  QuadMesh *wallMeshes[4];
  Room* rooms[4] = { nullptr,nullptr,nullptr,nullptr };
  Room(Vector3 pos, Vector3 scale);
  ~Room() {}
  virtual bool Contains(BBox* box) const override;
  float width() const;
  float depth() const;
  float height() const;
  Room* SpawnRoom(Direction dir);
  void Draw() const;

};
#endif

