#include "CubeMesh.h"
#include "Room.h"

Material CubeMesh::material = Material(Vector4(0, 0.05f, 0, 1), Vector4(0, 0, 0.004f, 1), Vector4(0, 0.5f, 0.5f, 1), 0);
Material CubeMesh::highlightMaterial = Material(Vector4(0, 0, 0, 1.0), Vector4(0, 0, 0, 1), Vector4(1, 0, 0, 1), 0);

bool CubeMesh::singleSelecting = true;

CubeMesh::CubeMesh(Room* rm):room(rm)
{
  
  selected = false;
  hovered = false;
  rotation.Set(0, 0, 0);
  center.Set(0, 0, 0);
  dim.Set(2.0f, 2.0f, 2.0f);
}


// Given a cube mesh, compute it's current bounding box and return in vectors min and max
// i.e. compute min.x,min.y,mi.z,max.x,max.y,max.z
// Use this function for collision detection of cube and walls/floor
BBox CubeMesh::getBBox() const
{
  //could be optimized by memoization

  Vector3 edgePoints[4] = {
    (Vector3(-dim.x, 0,-dim.z)*.5).GetRotatedY(rotation.y),
    (Vector3(-dim.x, 0,dim.z )*.5).GetRotatedY(rotation.y),
    (Vector3(dim.x, 0,-dim.z )*.5).GetRotatedY(rotation.y),
    (Vector3(dim.x, 0,dim.z  )*.5).GetRotatedY(rotation.y) };

  float maxX = FLT_MIN;
  float maxZ = FLT_MIN;

  for (auto pt : edgePoints) {
    if (pt.x > maxX) maxX = pt.x;
    if (pt.z > maxZ) maxZ = pt.z;
  }
  auto bounds = Vector3(maxX, dim.y/2, maxZ);
  auto b = BBox();
  b.min = (center - (bounds) );
  b.max = (center + (bounds) );
  return b;
}

void CubeMesh::drawSelector() const {
  glPushMatrix();

  if (singleSelecting) highlightMaterial.glApply();
  else material.glApply();

  glTranslatef(0, dim.y / 2 + 0.2, 0);
  glRotatef(270, 1, 0, 0);
  GLUquadricObj *quadObj = gluNewQuadric();
  gluCylinder(quadObj, 0, .5, 1, 10, 10);
  glTranslatef(0, 0, .5);
  gluCylinder(quadObj, .25, .25, 1, 10, 10);
  glPopMatrix();
}

void CubeMesh::drawCube() const
{
  // Transform and Draw cube   
  glPushMatrix();
  glPushAttrib(GL_LIGHTING_BIT);
  glTranslatef(center.x, center.y, center.z);
  if (hovered) drawSelector();
  glRotatef(rotation.y, 0, 1, 0);
  glScalef(dim.x/2, dim.y/2, dim.z/2);
  Material* matptr = selected ? &highlightMaterial : &material;

  for(auto& face : faces) {
    face.DrawMesh(matptr);
  }
  glPopMatrix();
  glPopAttrib();
}

bool CubeMesh::translate(Vector3 diff)
{
  center += diff*.4;
  BBox b = getBBox();
  center -= diff*.2;

  if( room->Contains(&b)) {
    if(!room->Contains(center)) {
      room = room->roomAt(center);
    }
    return true;
  }
  return room->Contains(center) && room->Contains(center + diff);
}

bool CubeMesh::scale(Vector3 diff)
{

  diff.x = diff.x == 1 ? 1.25f : diff.x == -1 ? 0.8f : 1;
  diff.z = diff.z == -1 ? 1.25f : diff.z == 1 ? 0.8f : 1;
  if ((rotation.y > 45 && rotation.y < 135) || (rotation.y > 225 && rotation.y < 315)) diff = Vector3(diff.z, 0, diff.x);
  dim.SetX(dim.x * diff.x);
  dim.SetZ(dim.z * diff.z);
  BBox b = getBBox();

  return room->Contains(&b) && dim.x > 0.1 && dim.z > 0.1;
}
bool CubeMesh::rotate(Vector3 diff)
{
  rotation.y = int(rotation.y + diff.y) % 360;
  BBox b = getBBox();
  return room->Contains(&b);
}
bool CubeMesh::rotateEulers(Vector3 rot)
{
	rotation.x = int(rotation.x + rot.x) % 360;
	rotation.y = int(rotation.y + rot.y) % 360;
	rotation.z = int(rotation.z + rot.z) % 360;
  BBox b = getBBox();
  return room->Contains(&b);
}
bool CubeMesh::extrude(Vector3 diff)
{
  dim.SetY(dim.y + diff.x - diff.z);
  center.SetY(center.y + (diff.x - diff.z)*.5);
  BBox b = getBBox();

  return room->Contains(&b) && dim.y > 0.1;;
}
bool CubeMesh::raise(Vector3 diff)
{
  center.SetY(center.y + (diff.x - diff.z)*.5);
  BBox b = getBBox();

  return room->Contains(&b);
}

Vector3 CubeMesh::Intersects(Ray ray) const {
  

  if (rotation.y != 0) {
    auto relorigin = ray.origin - center;
    auto reltarget = (ray.origin + ray.dir) - center;
    relorigin.RotateY(rotation.y);
    reltarget.RotateY(rotation.y);
    ray.origin = center + relorigin;
    ray.dir = (center + reltarget - ray.origin);
  }
  ray.origin -= center;

  ray.origin.z /= dim.z / 2;
  ray.origin.y /= dim.y / 2;
  ray.origin.x /= dim.x / 2;
  ray.dir.z /= dim.z/2;
  ray.dir.y /= dim.y / 2;
  ray.dir.x /= dim.x/2;

  Vector3 ret = Vector3::Sentinel();
  for (auto& face : faces) {
    auto hit = face.intersectsRay(ray);
    if (hit.isValid()) {
      hit.z *= dim.z/2;
      hit.x *= dim.x / 2;
      hit.y *= dim.y/2;
      hit.RotateY(-rotation.y);

      hit += center;
      if(!ret.isValid() || hit.z > ret.z) 
        ret = hit;
    }
  }
  return ret;
}
