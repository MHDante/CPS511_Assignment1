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
  scale *= 2;

	faces[0] =new QuadMesh(1, Vector3(-1, 1, -1), Vector3(0, 0, 2), Vector3(2, 0, 0), texture);
	faces[1] =new QuadMesh(1, Vector3(1, -1, 1), Vector3(-2, 0, 0), Vector3(0, 0, -2), texture);
	faces[2] =new QuadMesh(1, Vector3(-1, -1, -1), Vector3(0, 0, 2), Vector3(0, 2, 0), texture);
	faces[3] =new QuadMesh(1, Vector3(1, 1, 1), Vector3(0, -2, 0), Vector3(0, 0, -2), texture);
	faces[4] =new QuadMesh(1, Vector3(1, 1, 1), Vector3(-2, 0, 0), Vector3(0, -2, 0), texture);
	faces[5] =new QuadMesh(1, Vector3(-1, -1, -1), Vector3(0, 2, 0), Vector3(2, 0, 0), texture);
}


// Given a cube mesh, compute it's current bounding box and return in vectors min and max
// i.e. compute min.x,min.y,mi.z,max.x,max.y,max.z
// Use this function for collision detection of cube and walls/floor
BBox CubeMesh::getBBox() const
{
  //could be optimized by memoization

  Vector3 edgePoints[4] = {
    (Vector3(-scale.x, 0,-scale.z)*.5).GetRotatedY(rotation.y),
    (Vector3(-scale.x, 0,scale.z )*.5).GetRotatedY(rotation.y),
    (Vector3(scale.x, 0,-scale.z )*.5).GetRotatedY(rotation.y),
    (Vector3(scale.x, 0,scale.z  )*.5).GetRotatedY(rotation.y) };

  float maxX = FLT_MIN;
  float maxZ = FLT_MIN;

  for (auto pt : edgePoints) {
    if (pt.x > maxX) maxX = pt.x;
    if (pt.z > maxZ) maxZ = pt.z;
  }
  auto bounds = Vector3(maxX, scale.y/2, maxZ);
  auto b = BBox();
  b.min = (position - (bounds) );
  b.max = (position + (bounds) );
  return b;
}

void CubeMesh::drawSelector() const {
  glPushMatrix();

  if (singleSelecting) highlightMaterial.glApply();
  else material.glApply();

  glTranslatef(0, scale.y / 2 + 0.2f, 0);
  glRotatef(270, 1, 0, 0);
  GLUquadricObj *quadObj = gluNewQuadric();
  gluCylinder(quadObj, 0, .5, 1, 10, 10);
  glTranslatef(0, 0, .5);
  gluCylinder(quadObj, .25, .25, 1, 10, 10);
  glPopMatrix();
}

void CubeMesh::draw() const
{
  // Transform and Draw cube   
  glPushMatrix();
  glPushAttrib(GL_LIGHTING_BIT);
  glTranslatef(position.x, position.y, position.z);
  if (hovered) drawSelector();
  glRotatef(rotation.y, 0, 1, 0);
  glScalef(scale.x/2, scale.y/2, scale.z/2);
  Material* matptr = selected ? &highlightMaterial : &material;

  for(auto& face : faces) {
    face->DrawMesh(matptr);
  }
  glPopMatrix();
  glPopAttrib();
}

bool CubeMesh::translate(Vector3 diff)
{
  position += diff*.4f;
  return checkCollision(true);
}
bool CubeMesh::checkCollision(bool pointbased) {

  BBox b = getBBox();
  auto room = Game::instance->roomAt(position);
  bool roomCollisions =  room != nullptr && (room->Contains(&b) || (pointbased && room->Contains(position)));
  if (!roomCollisions) 
    return false;
  for(auto& blocks : Game::instance->cubes) {
    BBox other = blocks->getBBox();
    if (other.Intersects(getBBox()) && !(pointbased && !other.Contains(position))) {
      return false;
    }
  }
  
  return true;
}
bool CubeMesh::Scale(Vector3 diff)
{

  diff.x = diff.x == 1 ? 1.25f : diff.x == -1 ? 0.8f : 1;
  diff.z = diff.z == -1 ? 1.25f : diff.z == 1 ? 0.8f : 1;
  if ((rotation.y > 45 && rotation.y < 135) || (rotation.y > 225 && rotation.y < 315)) diff = Vector3(diff.z, 0, diff.x);
  scale.SetX(scale.x * diff.x);
  scale.SetZ(scale.z * diff.z);
  return checkCollision() && scale.x > 0.1 && scale.z > 0.1;
}
bool CubeMesh::rotate(Vector3 diff)
{
  rotation.y = float(fmod(rotation.y + diff.y , 360));
  return checkCollision();
}
bool CubeMesh::rotateEulers(Vector3 rot)
{
	rotation.x = float(fmod(rotation.x + rot.x,360));
	rotation.y = float(fmod(rotation.y + rot.y,360));
	rotation.z = float(fmod(rotation.z + rot.z,360));
  return checkCollision();
}
bool CubeMesh::extrude(Vector3 diff)
{
  scale.SetY(scale.y + diff.x - diff.z);
  position.SetY(position.y + (diff.x - diff.z)*.5f);
  return checkCollision() && scale.y > 0.1;;
}
bool CubeMesh::raise(Vector3 diff)
{
  position.SetY(position.y + (diff.x - diff.z)*.5f);
  return checkCollision();
}

Vector3 CubeMesh::Intersects(Ray ray) const {
  

  if (rotation.y != 0) {
    auto relorigin = ray.origin - position;
    auto reltarget = (ray.origin + ray.dir) - position;
    relorigin.RotateY(rotation.y);
    reltarget.RotateY(rotation.y);
    ray.origin = position + relorigin;
    ray.dir = (position + reltarget - ray.origin);
  }
  ray.origin -= position;

  ray.origin.z /= scale.z / 2;
  ray.origin.y /= scale.y / 2;
  ray.origin.x /= scale.x / 2;
  ray.dir.z /= scale.z/2;
  ray.dir.y /= scale.y / 2;
  ray.dir.x /= scale.x/2;

  Vector3 ret = Vector3::Sentinel();
  for (auto& face : faces) {
    auto hit = face->intersectsRay(ray);
    if (hit.isValid()) {
      hit.z *= scale.z/2;
      hit.x *= scale.x / 2;
      hit.y *= scale.y/2;
      hit.RotateY(-rotation.y);

      hit += position;
      if(!ret.isValid() || hit.z > ret.z) 
        ret = hit;
    }
  }
  return ret;
}
void CubeMesh::update(int deltaTime) {}