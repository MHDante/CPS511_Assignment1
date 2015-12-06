#include "CubeMesh.h"
#include "Room.h"
#include "Game.h"

Material CubeMesh::material = Material(Vector4(0, 0.05f, 0, 1), Vector4(0, 0, 0.004f, 1), Vector4(0, 0.5f, 0.5f, 1), 0);
Material CubeMesh::highlightMaterial = Material(Vector4(0, 0, 0, 1.0), Vector4(0, 0, 0, 1), Vector4(1, 0, 0, 1), 0);

bool CubeMesh::singleSelecting = true;

CubeMesh::CubeMesh(Textures texture) : texture(texture)
{
  selected = false;
  hovered = false;
	faces[0] =new QuadMesh(1, Vector3(-.5, .5, -.5), Vector3(0, 0, 1), Vector3(1, 0, 0), texture);
	faces[1] =new QuadMesh(1, Vector3(.5, -.5, .5), Vector3(-1, 0, 0), Vector3(0, 0, -1), texture);
	faces[2] =new QuadMesh(1, Vector3(-.5, -.5, -.5), Vector3(0, 0, 1), Vector3(0, 1, 0), texture);
	faces[3] =new QuadMesh(1, Vector3(.5, .5, .5), Vector3(0, -1, 0), Vector3(0, 0, -1), texture);
	faces[4] =new QuadMesh(1, Vector3(.5, .5, .5), Vector3(-1, 0, 0), Vector3(0, -1, 0), texture);
	faces[5] =new QuadMesh(1, Vector3(-.5, -.5, -.5), Vector3(0, 1, 0), Vector3(1, 0, 0), texture);
}



void CubeMesh::drawSelf() const
{
  // Transform and Draw mesh   

  glPushAttrib(GL_LIGHTING_BIT);
  Material* matptr = selected ? &highlightMaterial : &material;
  for(auto& face : faces) {
    face->DrawMesh(matptr);
  }
  glPopAttrib();

}


bool CubeMesh::checkCollision(bool pointbased) {

  BBox b = getBBox();
  auto room = Game::instance->roomAt(getWorldPos());
  bool roomCollisions =  room != nullptr && room->Contains(&b, pointbased);
  if (!roomCollisions) 
    return false;
  for(auto& blocks : Game::instance->cubes) {
    BBox other = blocks->getBBox();
    if (other.Intersects(getBBox()) && !(pointbased && !other.Contains(getWorldPos()))) {
      return false;
    }
  }
  
  return true;
}


Vector3 CubeMesh::Intersects(Ray ray) const {
  

  if (getRotation().y != 0) {
    auto relorigin = ray.origin - getWorldPos();
    auto reltarget = (ray.origin + ray.dir) - getWorldPos();
    relorigin.RotateY(getRotation().y);
    reltarget.RotateY(getRotation().y);
    ray.origin = getWorldPos() + relorigin;
    ray.dir = (getWorldPos() + reltarget - ray.origin);
  }
  ray.origin -= getWorldPos();

  ray.origin.z /= getScale().z / 2;
  ray.origin.y /= getScale().y / 2;
  ray.origin.x /= getScale().x / 2;
  ray.dir.z /= getScale().z/2;
  ray.dir.y /= getScale().y / 2;
  ray.dir.x /= getScale().x/2;

  Vector3 ret = Vector3::Sentinel();
  for (auto& face : faces) {
    auto hit = face->intersectsRay(ray);
    if (hit.isValid()) {
      hit.z *= getScale().z/2;
      hit.x *= getScale().x / 2;
      hit.y *= getScale().y/2;
      hit.RotateY(-getRotation().y);

      hit += getWorldPos();
      if(!ret.isValid() || hit.z > ret.z) 
        ret = hit;
    }
  }
  return ret;
}
void CubeMesh::update(int deltaTime) {}