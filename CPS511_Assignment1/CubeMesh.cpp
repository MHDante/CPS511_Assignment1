#include "CubeMesh.h"

Vector4 CubeMesh::mat_ambient = Vector4(0, 0.05f, 0, 1);
Vector4 CubeMesh::mat_specular = Vector4(0, 0, 0.004f, 1);
Vector4 CubeMesh::mat_diffuse = Vector4(0.5f, 0.5f, 0.5f, 1);
float CubeMesh::mat_shininess = 0;

Vector4 CubeMesh::highlightMat_ambient = Vector4(0, 0, 0, 1.0);
Vector4 CubeMesh::highlightMat_specular = Vector4(0, 0, 0, 1);
Vector4 CubeMesh::highlightMat_diffuse = Vector4(1, 0, 0, 1);
float CubeMesh::highlightMat_shininess = 0;
bool CubeMesh::singleSelecting = true;

CubeMesh::CubeMesh()
{
  selected = false;
  hovered = false;
  angle = 0;
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
    Vector3(-dim.x, 0,-dim.z).GetRotatedY(angle),
    Vector3(-dim.x, 0,dim.z ).GetRotatedY(angle),
    Vector3(dim.x, 0,-dim.z ).GetRotatedY(angle),
    Vector3(dim.x, 0,dim.z  ).GetRotatedY(angle) };

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



bool CubeMesh::isWithin(BBox* a) const
{
  auto b = getBBox();
  bool res = ((a->min.x <= b.min.x) && (a->min.y <= b.min.y) && (a->min.z <= b.min.z) &&
    (a->max.x >= b.max.x) && (a->max.y >= b.max.y) && (a->max.z >= b.max.z));
  return res;
}

void CubeMesh::drawSelector() const {
  glPushMatrix();

  glMaterialfv(GL_FRONT, GL_AMBIENT,  singleSelecting?  highlightMat_ambient    : mat_ambient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, singleSelecting ? highlightMat_specular : mat_specular);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, singleSelecting ? highlightMat_diffuse : mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SHININESS, singleSelecting ? &highlightMat_shininess : &mat_shininess);
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
  glRotatef(angle, 0, 1, 0);
  glScalef(dim.x/2, dim.y/2, dim.z/2);


  glMaterialfv(GL_FRONT, GL_AMBIENT, selected ? highlightMat_ambient : mat_ambient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, selected ? highlightMat_specular : mat_specular);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, selected ? highlightMat_diffuse : mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SHININESS, selected ? &highlightMat_shininess : &mat_shininess);


  glBegin(GL_QUADS);
  // Back Face
  glNormal3f(quadNormals[0].x, quadNormals[0].y, quadNormals[0].z);
  glVertex3f(vertices[quads[0]].x, vertices[quads[0]].y, vertices[quads[0]].z);
  glVertex3f(vertices[quads[1]].x, vertices[quads[1]].y, vertices[quads[1]].z);
  glVertex3f(vertices[quads[2]].x, vertices[quads[2]].y, vertices[quads[2]].z);
  glVertex3f(vertices[quads[3]].x, vertices[quads[3]].y, vertices[quads[3]].z);
  // Top Face
  glNormal3f(quadNormals[1].x, quadNormals[1].y, quadNormals[1].z);
  glVertex3f(vertices[quads[4]].x, vertices[quads[4]].y, vertices[quads[4]].z);
  glVertex3f(vertices[quads[5]].x, vertices[quads[5]].y, vertices[quads[5]].z);
  glVertex3f(vertices[quads[6]].x, vertices[quads[6]].y, vertices[quads[6]].z);
  glVertex3f(vertices[quads[7]].x, vertices[quads[7]].y, vertices[quads[7]].z);
  // Left Face
  glNormal3f(quadNormals[2].x, quadNormals[2].y, quadNormals[2].z);
  glVertex3f(vertices[quads[8]].x, vertices[quads[8]].y, vertices[quads[8]].z);
  glVertex3f(vertices[quads[9]].x, vertices[quads[9]].y, vertices[quads[9]].z);
  glVertex3f(vertices[quads[10]].x, vertices[quads[10]].y, vertices[quads[10]].z);
  glVertex3f(vertices[quads[11]].x, vertices[quads[11]].y, vertices[quads[11]].z);
  // Right Face
  glNormal3f(quadNormals[3].x, quadNormals[3].y, quadNormals[3].z);
  glVertex3f(vertices[quads[12]].x, vertices[quads[12]].y, vertices[quads[12]].z);
  glVertex3f(vertices[quads[13]].x, vertices[quads[13]].y, vertices[quads[13]].z);
  glVertex3f(vertices[quads[14]].x, vertices[quads[14]].y, vertices[quads[14]].z);
  glVertex3f(vertices[quads[15]].x, vertices[quads[15]].y, vertices[quads[15]].z);
  // Front Face
  glNormal3f(quadNormals[4].x, quadNormals[4].y, quadNormals[4].z);
  glVertex3f(vertices[quads[16]].x, vertices[quads[16]].y, vertices[quads[16]].z);
  glVertex3f(vertices[quads[17]].x, vertices[quads[17]].y, vertices[quads[17]].z);
  glVertex3f(vertices[quads[18]].x, vertices[quads[18]].y, vertices[quads[18]].z);
  glVertex3f(vertices[quads[19]].x, vertices[quads[19]].y, vertices[quads[19]].z);
  // Bottom Face
  glNormal3f(quadNormals[5].x, quadNormals[5].y, quadNormals[5].z);
  glVertex3f(vertices[quads[20]].x, vertices[quads[20]].y, vertices[quads[20]].z);
  glVertex3f(vertices[quads[21]].x, vertices[quads[21]].y, vertices[quads[21]].z);
  glVertex3f(vertices[quads[22]].x, vertices[quads[22]].y, vertices[quads[22]].z);
  glVertex3f(vertices[quads[23]].x, vertices[quads[23]].y, vertices[quads[23]].z);
  glEnd();
  glPopMatrix();
  glPopAttrib();
}

bool CubeMesh::translate(Vector3 diff, BBox* bounds)
{
  center += diff*.2;
  return isWithin(bounds);
}

bool CubeMesh::scale(Vector3 diff, BBox* bounds)
{

  diff.x = diff.x == 1 ? 1.25f : diff.x == -1 ? 0.8f : 1;
  diff.z = diff.z == -1 ? 1.25f : diff.z == 1 ? 0.8f : 1;
  if ((angle > 45 && angle < 135) || (angle > 225 && angle < 315)) diff = Vector3(diff.z, 0, diff.x);
  dim.SetX(dim.x * diff.x);
  dim.SetZ(dim.z * diff.z);
  return isWithin(bounds) && dim.x > 0.1 && dim.z > 0.1;
}
bool CubeMesh::rotate(Vector3 diff, BBox* bounds)
{
  diff *= 5;
  angle = int(angle + diff.x + diff.z) % 360;
  return isWithin(bounds);
}
bool CubeMesh::extrude(Vector3 diff, BBox* bounds)
{
  dim.SetY(dim.y + diff.x - diff.z);
  center.SetY(center.y + (diff.x - diff.z)*.5);

  return isWithin(bounds) && dim.y > 0.1;;
}
bool CubeMesh::raise(Vector3 diff, BBox* bounds)
{
  center.SetY(center.y + (diff.x - diff.z)*.5);

  return isWithin(bounds);
}
