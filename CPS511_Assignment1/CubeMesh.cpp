#include "CubeMesh.h"

bool CubeMesh::singleSelecting = true;

CubeMesh::CubeMesh()
{
  selected = false;
  hovered = false;
  angle = 0;
  center.Set(0, 0, 0);
  dim.Set(2.0f, 2.0f, 2.0f);

  mat_ambient[0] = 0.0;
  mat_ambient[1] = 0.05;
  mat_ambient[2] = 0.0;
  mat_ambient[3] = 1.0;
  mat_specular[0] = 0.0;
  mat_specular[1] = 0.0;
  mat_specular[2] = 0.004;
  mat_specular[3] = 1.0;
  mat_diffuse[0] = 0.5;
  mat_diffuse[1] = 0.5;
  mat_diffuse[2] = 0.5;
  mat_diffuse[3] = 1.0;
  mat_shininess[0] = 0;

  highlightMat_ambient[0] = 0.0;
  highlightMat_ambient[1] = 0.00;
  highlightMat_ambient[2] = 0.0;
  highlightMat_ambient[3] = 1.0;
  highlightMat_specular[0] = 0.0;
  highlightMat_specular[1] = 0.0;
  highlightMat_specular[2] = 0.0;
  highlightMat_specular[3] = 1.0;
  highlightMat_diffuse[0] = 1.0;
  highlightMat_diffuse[1] = 0.0;
  highlightMat_diffuse[2] = 0.0;
  highlightMat_diffuse[3] = 1.0;
  highlightMat_shininess[0] = 0.0;
}

Vector3 rotatePoint(Vector3 v, float angle) {
  return Vector3(
    (v.x / 2) * cos(angle) - (v.z / 2) * sin(angle),
    v.y,
    (v.z / 2) * cos(angle) + (v.x / 2) * sin(angle));
}

// Given a cube mesh, compute it's current bounding box and return in vectors min and max
// i.e. compute min.x,min.y,mi.z,max.x,max.y,max.z
// Use this function for collision detection of cube and walls/floor
BBox* CubeMesh::getBBox() const
{
  Vector3 edgePoints[4] = {
    rotatePoint(Vector3(-dim.x, 0,-dim.z), angle),
    rotatePoint(Vector3(-dim.x, 0,dim.z), angle),
    rotatePoint(Vector3(dim.x, 0,-dim.z), angle),
    rotatePoint(Vector3(dim.x, 0,dim.z), angle) };

  float maxX = FLT_MIN;
  float maxZ = FLT_MIN;

  for (auto pt : edgePoints) {
    if (pt.x > maxX) maxX = pt.x;
    if (pt.z > maxZ) maxZ = pt.z;
  }
  auto bounds = Vector3(maxX, dim.y/2, maxZ);
  auto b = new BBox();
  b->min = (center - (bounds) );
  b->max = (center + (bounds) );
  return b;
}



bool CubeMesh::isWithin(BBox* a) const
{


  auto b = getBBox();
  bool res = ((a->min.x <= b->min.x) && (a->min.y <= b->min.y) && (a->min.z <= b->min.z) &&
    (a->max.x >= b->max.x) && (a->max.y >= b->max.y) && (a->max.z >= b->max.z));
  delete b;
  return res;
}

void CubeMesh::drawCube() const
{
  // Transform and Draw cube   
  glPushMatrix();
  glPushAttrib(GL_LIGHTING_BIT);
  glTranslatef(center.x, center.y, center.z);

  if (hovered) {
    if (singleSelecting)
    {
      // Setup the material and lights used for the cube
      glMaterialfv(GL_FRONT, GL_AMBIENT, highlightMat_ambient);
      glMaterialfv(GL_FRONT, GL_SPECULAR, highlightMat_specular);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, highlightMat_diffuse);
      glMaterialfv(GL_FRONT, GL_SHININESS, highlightMat_shininess);
    }
    else
    {
      // Setup the material and lights used for the cube
      glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
      glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
      glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    }

    glPushMatrix();
    glTranslatef(0, dim.y / 2 + 0.2, 0);
    glRotatef(270, 1, 0, 0);
    GLUquadricObj *quadObj = gluNewQuadric();
    gluCylinder(quadObj, 0, .5, 1, 10, 10);
    glTranslatef(0, 0, .5);
    gluCylinder(quadObj, .25, .25, 1, 10, 10);

    glPopMatrix();
  }

  glRotatef(angle, 0, 1, 0);

  glScalef(dim.x/2, dim.y/2, dim.z/2);

  
  

  if (selected)
  {
    // Setup the material and lights used for the cube
    glMaterialfv(GL_FRONT, GL_AMBIENT, highlightMat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, highlightMat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, highlightMat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, highlightMat_shininess);
  }
  else
  {
    // Setup the material and lights used for the cube
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  }


  glBegin(GL_QUADS);
  // Back Face
  glNormal3f(quadNormals[0][0], quadNormals[0][1], quadNormals[0][2]);
  glVertex3f(vertices[quads[0]][0], vertices[quads[0]][1], vertices[quads[0]][2]);
  glVertex3f(vertices[quads[1]][0], vertices[quads[1]][1], vertices[quads[1]][2]);
  glVertex3f(vertices[quads[2]][0], vertices[quads[2]][1], vertices[quads[2]][2]);
  glVertex3f(vertices[quads[3]][0], vertices[quads[3]][1], vertices[quads[3]][2]);
  // Top Face
  glNormal3f(quadNormals[1][0], quadNormals[1][1], quadNormals[1][2]);
  glVertex3f(vertices[quads[4]][0], vertices[quads[4]][1], vertices[quads[4]][2]);
  glVertex3f(vertices[quads[5]][0], vertices[quads[5]][1], vertices[quads[5]][2]);
  glVertex3f(vertices[quads[6]][0], vertices[quads[6]][1], vertices[quads[6]][2]);
  glVertex3f(vertices[quads[7]][0], vertices[quads[7]][1], vertices[quads[7]][2]);
  // Left Face
  glNormal3f(quadNormals[2][0], quadNormals[2][1], quadNormals[2][2]);
  glVertex3f(vertices[quads[8]][0], vertices[quads[8]][1], vertices[quads[8]][2]);
  glVertex3f(vertices[quads[9]][0], vertices[quads[9]][1], vertices[quads[9]][2]);
  glVertex3f(vertices[quads[10]][0], vertices[quads[10]][1], vertices[quads[10]][2]);
  glVertex3f(vertices[quads[11]][0], vertices[quads[11]][1], vertices[quads[11]][2]);
  // Right Face
  glNormal3f(quadNormals[3][0], quadNormals[3][1], quadNormals[3][2]);
  glVertex3f(vertices[quads[12]][0], vertices[quads[12]][1], vertices[quads[12]][2]);
  glVertex3f(vertices[quads[13]][0], vertices[quads[13]][1], vertices[quads[13]][2]);
  glVertex3f(vertices[quads[14]][0], vertices[quads[14]][1], vertices[quads[14]][2]);
  glVertex3f(vertices[quads[15]][0], vertices[quads[15]][1], vertices[quads[15]][2]);
  // Front Face
  glNormal3f(quadNormals[4][0], quadNormals[4][1], quadNormals[4][2]);
  glVertex3f(vertices[quads[16]][0], vertices[quads[16]][1], vertices[quads[16]][2]);
  glVertex3f(vertices[quads[17]][0], vertices[quads[17]][1], vertices[quads[17]][2]);
  glVertex3f(vertices[quads[18]][0], vertices[quads[18]][1], vertices[quads[18]][2]);
  glVertex3f(vertices[quads[19]][0], vertices[quads[19]][1], vertices[quads[19]][2]);
  // Bottom Face
  glNormal3f(quadNormals[5][0], quadNormals[5][1], quadNormals[5][2]);
  glVertex3f(vertices[quads[20]][0], vertices[quads[20]][1], vertices[quads[20]][2]);
  glVertex3f(vertices[quads[21]][0], vertices[quads[21]][1], vertices[quads[21]][2]);
  glVertex3f(vertices[quads[22]][0], vertices[quads[22]][1], vertices[quads[22]][2]);
  glVertex3f(vertices[quads[23]][0], vertices[quads[23]][1], vertices[quads[23]][2]);
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
