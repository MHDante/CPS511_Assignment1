#include "Room.h"
#include "CubeMesh.h"
#include "Game.h"

Room::Room(Vector3 pos, Vector3 Scale) {

  setPosition(pos);
  setScale(Scale);
  min = pos - Scale / 2;
  max = pos + Scale / 2;
  
  Vector3 min = Vector3(-.5f,-.5f,-.5f);
  Vector3 max = Vector3(.5f,.5f,.5f);

  Vector3 up = Vector3(UP);
  Vector3 right = Vector3(RIGHT);
  Vector3 forward = Vector3(FORWARD);

  // Set up meshes
  Vector4 diffuse = Vector4(0.9f, 0.5f, 0.0f, 1);
  wallMeshes[DOWN] = new QuadMesh(meshSize, min, forward, right, Textures::PROFESSOR);
  wallMeshes[UP] = new QuadMesh(meshSize, max, -right, -forward, Textures::PROFESSOR);
  wallMeshes[LEFT] = new QuadMesh(meshSize, min, up, forward, Textures::TILES01);
  wallMeshes[BACK] = new QuadMesh(meshSize, min, right, up, Textures::TILES01);
  wallMeshes[RIGHT] = new QuadMesh(meshSize, max, -forward, -up, Textures::TILES01);
  wallMeshes[FORWARD] = new QuadMesh(meshSize, max, -up, -right, Textures::TILES01);
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
float Room::width() const { return max.x - min.x; }
float Room::depth() const { return max.z - min.z; }
float Room::height() const { return max.y - min.y; }


Room* Room::SpawnRoom(Direction dir) {
  if (rooms[int(dir)] != nullptr) return rooms[int(dir)];
  

    CubeMesh* wall1 = new CubeMesh(Textures::TILES01);
    CubeMesh* wall2 = new CubeMesh(Textures::TILES01);
    auto doorWidth = .25;
    float ratio = randZeroToOne();
    auto pos = Vector3(dir);
    auto unit = (dir == UP || dir == DOWN) ? Vector3(FORWARD) : Vector3(UP);
    auto edge = (dir == FORWARD || dir == BACK) ? Vector3(RIGHT) : Vector3(FORWARD);

    auto leeway =  (1 - doorWidth);
    wall1->setPosition(pos/2 + edge * (-.5 + (leeway * ratio /2)));
    wall2->setPosition(pos/2 + edge * (.5 - (leeway * (1 - ratio) /2)));
    wall1->setScale( unit + edge * (leeway * ratio) + pos * 0.05f);
    wall2->setScale(unit + edge * (leeway * (1 - ratio)) + pos * 0.05f);
    addChild(wall1);
    addChild(wall2);
    rooms[int(dir)] = new Room(getPosition() + pos.ElementWiseProduct(getScale()), getScale());
    rooms[int(dir)]->rooms[int(Opposite(dir))] = this;
    return rooms[int(dir)];
}

void Room::drawSelf() const {
  // Draw floor and wall meshes
  for (int i = 0; i < 6; i++)
  {
    if(rooms[i] == nullptr)
      wallMeshes[i]->DrawMesh();
  }
}
