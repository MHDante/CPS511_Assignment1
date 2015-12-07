#include "Room.h"
#include "CubeMesh.h"
#include "Game.h"

Room::Room(Vector3 pos, Vector3 Scale) {

  setPosition(pos);
  setScale(Scale);
  
  Vector3 min = Vector3(-.5f,-.5f,-.5f);
  Vector3 max = Vector3(.5f,.5f,.5f);

  Vector3 up = Vector3(UP);
  Vector3 right = Vector3(RIGHT);
  Vector3 forward = Vector3(FORWARD);

  // Set up meshes
  Vector4 diffuse = Vector4(0.9f, 0.5f, 0.0f, 1);
  wallMeshes[DOWN] = new QuadMesh(meshSize, min, forward, right, Textures::PLANK);
  wallMeshes[UP] = new QuadMesh(meshSize, max, -right, -forward, Textures::CLOVER);
  wallMeshes[LEFT] = new QuadMesh(meshSize, min, up, forward, Textures::TILES01);
  wallMeshes[BACK] = new QuadMesh(meshSize, min, right, up, Textures::TILES01);
  wallMeshes[RIGHT] = new QuadMesh(meshSize, max, -forward, -up, Textures::TILES01);
  wallMeshes[FORWARD] = new QuadMesh(meshSize, max, -up, -right, Textures::TILES01);
}

bool Room::Contains(BBox* box, bool pointbased) const {

  bool res = 
    ((box->min.x >= bounds.min.x || rooms[LEFT] != nullptr) &&
    (box->min.y >= bounds.min.y) &&
    (box->min.z >= bounds.min.z || rooms[FORWARD] != nullptr) &&
    (box->max.x <= bounds.max.x || rooms[RIGHT] != nullptr) &&
    (box->max.y <= bounds.max.y) &&
    (box->max.z <= bounds.max.z || rooms[BACK] != nullptr));
  res =  (res || (pointbased && bounds.Contains(getWorldPos())));
  return res;

}

void Room::drawChildren() const {
  
}

Room* Room::SpawnRoom(Direction dir) {
  if (rooms[int(dir)] != nullptr) return rooms[int(dir)];
  

    CubeMesh* wall1 = new CubeMesh(Textures::TILES01);
    CubeMesh* wall2 = new CubeMesh(Textures::TILES01);
    float doorWidth = .25;
    float ratio = randZeroToOne();
    auto pos = Vector3(dir);
    auto straight = (dir == FORWARD || dir == BACK) ? Vector3(FORWARD) : Vector3(RIGHT);

    auto unit = (dir == UP || dir == DOWN) ? Vector3(FORWARD) : Vector3(UP);
    auto edge = (dir == FORWARD || dir == BACK) ? Vector3(RIGHT) : Vector3(FORWARD);

    float leeway =  (1 - doorWidth);

    addChild(wall1);
    addChild(wall2);
    wall1->setPosition(pos/2 + edge * (-.5f + (leeway * ratio /2)));
    wall2->setPosition(pos/2 + edge * (.5f - (leeway * (1 - ratio) /2)));
    wall1->setScale( unit + edge * (leeway * ratio) + straight * 0.05f);
    wall2->setScale(unit + edge * (leeway * (1 - ratio)) + straight * 0.05f);

    Game::instance->cubes.push_back(wall1);//Ughhhhh.
    Game::instance->cubes.push_back(wall2);//Ughhhhh.
    rooms[int(dir)] = new Room(getWorldPos() + pos.ElementWiseProduct(getScale()), getScale());
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

void Room::updateGeometry() {
  bounds = getBBox();

}
