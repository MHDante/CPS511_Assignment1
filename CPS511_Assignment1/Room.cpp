#include "Room.h"
#include "CubeMesh.h"
#include "Game.h"
std::unordered_set<Room*> Room::allRooms = std::unordered_set<Room*>();

Room::Room(Vector3 min, Vector3 max) :BBox(min,max) {

  Vector3 up = Vector3(0, max.y - min.y, 0);
  Vector3 right = Vector3(max.x-min.x, 0, 0);
  Vector3 forward = Vector3(0, 0, max.z - min.z);

  // Set up meshes
  Vector4 diffuse = Vector4(0.9f, 0.5f, 0.0f, 1);
  floorMesh = new QuadMesh(meshSize, min, forward, right, Textures::PROFESSOR);
  floorMesh->material.diffuse = diffuse;

  diffuse = Vector4(0, 1, 0.0f, 4);
  wallMeshes[LEFT] = new QuadMesh(meshSize, min, up, forward, Textures::TILES01);
  wallMeshes[LEFT]->material.diffuse = diffuse;
  wallMeshes[FORWARD] = new QuadMesh(meshSize, min, right, up, Textures::TILES01);
  wallMeshes[FORWARD]->material.diffuse = diffuse;
  wallMeshes[RIGHT] = new QuadMesh(meshSize, max, -forward, -up, Textures::TILES01);
  wallMeshes[RIGHT]->material.diffuse = diffuse;
  wallMeshes[BACK] = new QuadMesh(meshSize, max, -up, -right, Textures::TILES01);
  wallMeshes[BACK]->material.diffuse = diffuse;
  allRooms.insert(this);
}

Room* Room::roomAt(Vector3 center) {
  for(auto& r : allRooms) {
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
float Room::width() const { return max.x - min.x; }
float Room::depth() const { return max.z - min.z; }
float Room::height() const { return max.y - min.y; }
Vector3 Room::center() const {
  
  return (max+min)/2;
}

Room* Room::SpawnRoom(Direction dir) {
  if (rooms[dir] != nullptr) return rooms[dir];
  
  Vector3 diff = Vector3(0,0,0);
    switch(dir) {
    case LEFT: diff.SetX(width());    break;
    case RIGHT: diff.SetX(-width()); break;
    case FORWARD: diff.SetZ(depth()); break;
    case BACK: diff.SetZ(-depth());    break;
  }

    CubeMesh* wall1 = new CubeMesh(Textures::TILES01);
    CubeMesh* wall2 = new CubeMesh(Textures::TILES01);
    auto doorWidth = 2;
    float ratio = randZeroToOne();
    Vector3 leeway, start, end;
    if (dir == LEFT || dir == RIGHT) {
      start = center() + (Vector3(dir == LEFT ? -width() : width(), 0, -depth()) / 2);
      end = center() + (Vector3(dir == LEFT ? -width() : width(),0,depth()) / 2);
      leeway = Vector3(0, 0, 1) * (depth() - doorWidth);
      wall1->dim = leeway * ratio + Vector3(.3, height(), 0);
      wall2->dim = leeway * (1-ratio) + Vector3(.3, height(), 0);

    } else {
      start = center() + (Vector3(-width(), 0, dir == BACK ? depth() : -depth()) / 2);
      end = center() + (Vector3(width(), 0, dir == BACK ? depth() : -depth()) / 2);
      leeway = Vector3(1, 0,0) * (width() - doorWidth);

      wall1->dim = leeway * ratio + Vector3(0, height(), .3);
      wall2->dim = leeway * (1 - ratio) + Vector3(0, height(), .3);
    }
    wall1->center = start + leeway*(ratio/2);
    wall2->center = end - leeway*((1 - ratio)/2);
    Game::instance->cubes.push_back(wall1);
    Game::instance->cubes.push_back(wall2);
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
