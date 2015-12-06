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
  int meshSize = 16;
  QuadMesh *wallMeshes[6];
  Room* rooms[6] = { nullptr,nullptr,nullptr,nullptr, nullptr, nullptr };
  Room(Vector3 pos, Vector3 scale);
  ~Room() {}
  virtual bool Contains(BBox* box) const override;
  float width() const;
  float depth() const;
  float height() const;
  Room* SpawnRoom(Direction dir);
  void drawSelf() const override;

};
#endif

