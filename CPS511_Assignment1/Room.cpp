#include "Room.h"

Room::Room(Vector3 min, Vector3 max) :BBox(min,max) {

  Vector3 up = Vector3(0, max.y - min.y, 0);
  Vector3 right = Vector3(max.x-min.x, 0, 0);
  Vector3 forward = Vector3(0, 0, max.z - min.z);

  // Set up meshes
  Vector4 diffuse = Vector4(0.9f, 0.5f, 0.0f, 1);
  floorMesh = new QuadMesh(meshSize, min, forward, right);
  floorMesh->material.diffuse = diffuse;

  diffuse = Vector4(0, 1, 0.0f, 4);
  wallMeshes[LEFT] = new QuadMesh(meshSize, min, up, forward);
  wallMeshes[LEFT]->material.diffuse = diffuse;
  wallMeshes[FORWARD] = new QuadMesh(meshSize, min, right, up);
  wallMeshes[FORWARD]->material.diffuse = diffuse;
  wallMeshes[RIGHT] = new QuadMesh(meshSize, max, -forward, -up);
  wallMeshes[RIGHT]->material.diffuse = diffuse;
  wallMeshes[BACK] = new QuadMesh(meshSize, max, -up, -right);
  wallMeshes[BACK]->material.diffuse = diffuse;
}

Room* Room::roomAt(Vector3 center) {
  for(auto& r : rooms) {
    if (r!= nullptr && r->Contains(center)) return r;
  }
  return nullptr;
}

bool Room::Contains(BBox* box) const {

  bool res = 
    ((box->min.x >= min.x || rooms[LEFT] != nullptr) &&
    (box->min.y >= min.y) &&
    (box->min.z >= min.z || rooms[FORWARD] != nullptr) &&
    (box->max.x <= max.x || rooms[RIGHT] != nullptr) &&
    (box->max.y <= max.y) &&
    (box->max.z <= max.z || rooms[BACK] != nullptr));
  return res;
}

Room* Room::SpawnRoom(Direction dir) {
  if (rooms[dir] != nullptr) return rooms[dir];

  Vector3 diff = Vector3(0,0,0);
    switch(dir) {
    case LEFT: diff.SetX(max.x - min.x);    break;
    case RIGHT: diff.SetX(min.x - max.x); break;
    case FORWARD: diff.SetZ(max.z - min.z); break;
    case BACK: diff.SetZ(min.z - max.z);    break;
  }

    rooms[dir] = new Room(min - diff, max - diff);
    Direction opp = Direction((dir + 2) % 4);
    rooms[dir]->rooms[opp] = this;
    return rooms[dir];
}

void Room::Draw() const {
  // Draw floor and wall meshes
  floorMesh->DrawMesh();
  for (int i = 0; i < 4; i++)
  {
    if(rooms[i] == nullptr)
      wallMeshes[i]->DrawMesh();
  }
}
