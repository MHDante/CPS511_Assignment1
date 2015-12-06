#pragma once
#include "GLUtils.h"
#include <set>
#include "Matrices.h"

class Transform
{
private:
  Transform* parent;

  Vector3 position = Vector3(0, 0, 0);
  Vector3 scale = Vector3(1, 1, 1);
  Vector3 rotation = Vector3(0, 0, 0);

protected:
  Matrix4 currentMatrix;
  Matrix4 inverseMatrix;

  virtual void updateMatrix();
  virtual void updateGeometry();
  virtual void drawSelf() const;
  virtual void drawChildren() const;
public:

  Transform* getParent() const{ return parent;}
  virtual void draw() const;
  void translate(Vector3 diff);
  void Scale(Vector3 diff);
  void rotateEulers(Vector3 rot);
  bool setParent(Transform* p);
  bool addChild(Transform* p);
  std::set<Transform*> children;
  Transform();
  explicit Transform(Transform* parent);
  Vector3 getWorldPos() const;
  Vector3 getLocalPos() const;
  void setPosition(Vector3 position);
  Vector3 getScale() const;
  void setScale(Vector3 scale);
  Vector3 getRotation() const;
  void setRotation(Vector3 rotation);
  virtual ~Transform();
  virtual BBox getBBox() const;
};

