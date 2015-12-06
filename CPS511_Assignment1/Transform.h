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
  

  virtual void updateMatrix();
  
  virtual void drawChildren() const;
public:
  Matrix4 currentMatrix;
  Transform* getParent() const{ return parent;}
  virtual void draw() const;
  virtual void drawSelf() const;
  virtual void translate(Vector3 diff);
  void Scale(Vector3 diff);
  void rotateEulers(Vector3 rot);
  bool setParent(Transform* p);
  bool addChild(Transform* p);
  std::set<Transform*> children;
  Transform();
  explicit Transform(Transform* parent);
  Vector3 getPosition() const;
  void setPosition(Vector3 position);
  Vector3 getScale() const;
  void setScale(Vector3 scale);
  Vector3 getRotation() const;
  void setRotation(Vector3 rotation);
  virtual ~Transform();
};

