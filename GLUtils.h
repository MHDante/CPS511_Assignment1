
#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <windows.h>
#include <gl/gl.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <gl/glut.h>
# define M_PIl 3.14159265358979323846f
#define DEG2RAD (M_PIl / 180)

enum class Textures { TILES01, PROFESSOR, MEGAMAN, PLANK, CLOVER };

enum Direction { RIGHT, UP, FORWARD, LEFT, DOWN, BACK, };

inline Direction Opposite(Direction dir) {
  return Direction((int(dir) + 3) % 6);
}


inline std::string floatToSmallString(float f, int precision = 2) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(precision) << f;
  return ss.str();
}

struct Vector4 {
  float x; float y; float z;  float w;
  Vector4() :x(0), y(0), z(0), w(0) {};
  Vector4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {};

  operator float* () const { return (float*) this; }
  operator const float* () const { return (const float*) this; }
};
struct Vector2 {
  float x; float y;
  Vector2() :x(0), y(0){};
  Vector2(float x, float y) :x(x), y(y){};

  operator float* () const { return (float*) this; }
  operator const float* () const { return (const float*) this; }
};


struct Material {

  Vector4 ambient;
  Vector4 specular;
  Vector4 diffuse;
  GLfloat shininess;

  Material(Vector4 ambient_, Vector4 specular_,  Vector4 diffuse_,  GLfloat shininess_):
    ambient(ambient_), specular(specular_), diffuse(diffuse_), shininess(shininess_) {}

  Material() : shininess(0) {}

  void glApply() const {
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
  }
};


//////////////////////////////////////////////////////////////////////////////////////////
//	VECTOR3D.h
//	Class declaration for a 3d vector
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//	Modified:	8th November 2002	-	Changed Constructor layout
//									-	Some speed Improvements
//									-	Corrected Lerp
//				7th January 2003	-	Added QuadraticInterpolate
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

class Vector3
{
public:
	//constructors
	Vector3(void)	:	x(0.0f), y(0.0f), z(0.0f)
	{}

	Vector3(float newX, float newY, float newZ)	:	x(newX), y(newY), z(newZ)
	{}

  explicit Vector3(const float * rhs) : x(*rhs), y(*(rhs + 1)), z(*(rhs + 2))
  {}

  explicit Vector3(Direction dir) {
    switch (dir) {
    case LEFT:    *this = Vector3(-1, 0, 0);  break;
    case RIGHT:   *this = Vector3(1, 0, 0);   break;
    case UP:      *this = Vector3(0, 1, 0);   break;
    case DOWN:    *this = Vector3(0, -1, 0);  break;
    case FORWARD: *this = Vector3(0, 0, 1);   break;
    case BACK:    *this = Vector3(0, 0, -1);  break;
    }
	}
	Vector3(const Vector3 & rhs)	:	x(rhs.x), y(rhs.y), z(rhs.z)
	{}

	~Vector3() {}	//empty

	void Set(float newX, float newY, float newZ)
	{	x=newX;	y=newY;	z=newZ;	}
	void Set(Vector3 o)
	{	x=o.x;	y=o.y;	z=o.z;	}
	//Accessors kept for compatibility
	void SetX(float newX) {x = newX;}
	void SetY(float newY) {y = newY;}
	void SetZ(float newZ) {z = newZ;}

	float GetX() const {return x;}	//public accessor functions
	float GetY() const {return y;}	//inline, const
	float GetZ() const {return z;}

	void LoadZero(void)
	{	x=y=z=0.0f;	}
	void LoadOne(void)
	{	x=y=z=1.0f;	}


  Vector3 ElementWiseProduct(Vector3 o) const {
    return Vector3(x*o.x, y *o.y, z*o.z);
	}

  static Vector3 Sentinel() { return Vector3(NAN, NAN, NAN); }
  static Vector3 Zero()     { return Vector3(0,0,0); }
  static Vector3 One()      { return Vector3(1,1,1); }
	//vector algebra
	Vector3 CrossProduct(const Vector3 & rhs) const
	{	return Vector3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);	}

	float DotProduct(const Vector3 & rhs) const
	{	return x*rhs.x + y*rhs.y + z*rhs.z;	}
	
	void Normalize()
	{
		const float norm = float(sqrt(x*x+y*y+z*z));
		if(norm > 0)
		{
			x /= norm; y /= norm; z /= norm;
		}
	}

  Vector3 Normalized() const
	{
    Vector3 res = Vector3(*this);
    res.Normalize();
    return res;
	}

	
	float GetLength() const
	{	return float(sqrt((x*x)+(y*y)+(z*z)));	}
	
	float GetQuaddLength() const
	{	return (x*x)+(y*y)+(z*z);	}

	//rotations
	void RotateX(double angle);
	Vector3 GetRotatedX(double angle) const;
	void RotateY(double angle) {
    float cn = float(cos(angle*DEG2RAD));
    float sn = float(sin(angle*DEG2RAD)) ;
    auto tempx = x * cn - z * sn;
    auto tempz = z * cn + x * sn;
    x = tempx;
    z = tempz;
	};
	Vector3 GetRotatedY(double angle) const {

    auto x = Vector3(*this);
    x.RotateY(angle);
    return x;

	};
	void RotateZ(double angle);
	Vector3 GetRotatedZ(double angle) const;
	void RotateAxis(double angle, const Vector3 & axis);
	Vector3 GetRotatedAxis(double angle, const Vector3 & axis) const;

	//pack to [0,1] for color
	void PackTo01();
	Vector3 GetPackedTo01() const;

	//linear interpolate
	Vector3 lerp(const Vector3 & v2, float factor) const
	{	return (*this)*(1.0f-factor) + v2*factor;	}

	Vector3 QuadraticInterpolate(const Vector3 & v2, const Vector3 & v3, float factor) const
	{	return (*this)*(1.0f-factor)*(1.0f-factor) + v2*(2*factor*(1.0f-factor)) + v3*factor*factor;}


	//overloaded operators
	//binary operators
	Vector3 operator+(const Vector3 & rhs) const
	{	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);	}
	
	Vector3 operator-(const Vector3 & rhs) const
	{	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);	}

	Vector3 operator*(const float rhs) const
	{	return Vector3(x*rhs, y*rhs, z*rhs);	}
	
	Vector3 operator/(const float rhs) const
	{	return (rhs==0.0f) ? Vector3(0.0f, 0.0f, 0.0f) : Vector3(x / rhs, y / rhs, z / rhs);	}

	//multiply by a float, eg 3*v
  friend Vector3 operator*(float scaleFactor, const Vector3 & rhs) {
    return rhs * scaleFactor;
  }
  friend Vector3 operator/(float s, const Vector3 & rhs) {
    return Vector3(s / rhs.x, s / rhs.y, s / rhs.z);
  }
	//Add, subtract etc, saving the construction of a temporary
	void Add(const Vector3 & v2, Vector3 & result) const
	{
		result.x=x+v2.x;
		result.y=y+v2.y;
		result.z=z+v2.z;
	}

	void Subtract(const Vector3 & v2, Vector3 & result) const
	{
		result.x=x-v2.x;
		result.y=y-v2.y;
		result.z=z-v2.z;
	}

	bool operator==(const Vector3 & rhs) const;
	bool operator!=(const Vector3 & rhs) const
	{	return !((*this)==rhs);	}

	//self-add etc
	void operator+=(const Vector3 & rhs)
	{	x+=rhs.x;	y+=rhs.y;	z+=rhs.z;	}

	void operator-=(const Vector3 & rhs)
	{	x-=rhs.x;	y-=rhs.y;	z-=rhs.z;	}

	void operator*=(const float rhs)
	{	x*=rhs;	y*=rhs;	z*=rhs;	}
	
	void operator/=(const float rhs)
	{	if(rhs==0.0f)
			return;
		else
		{	x/=rhs; y/=rhs; z/=rhs;	}
	}


	//unary operators
	Vector3 operator-(void) const {return Vector3(-x, -y, -z);}
	Vector3 operator+(void) const {return *this;}

	//cast to pointer to a (float *) for glVertex3fv etc
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

  bool isValid() const { return !(isnan(x)||isnan(y)||isnan(z)); }
  std::string toString() const
  {



    return ("(" + floatToSmallString(x) + "," + floatToSmallString(y) + "," + floatToSmallString(z) +")");
	}
  //member variables
	float x;
	float y;
	float z;

};
//Why do I keep Three of these? 

struct Ray {
  Vector3 origin, dir;
  Ray(Vector3 origin, Vector3 dir) :origin(origin), dir(dir) {};
  Ray() :origin(Vector3()), dir(Vector3()) {};
  Vector3 ptAt(float dist) const {
    return origin + dir * dist;
  }
};

struct Line
{
  Vector3 to,from;
  Vector4 color = Vector4(0,0,1,1);
  Line(Vector3 to, Vector3 from) :to(to), from(from) {}

  void Draw() const {
    glDisable(GL_LIGHTING);
    glColor3f(color.x, color.y, color.z);
    glBegin(GL_LINES);

    glVertex3f(from.x, from.y, from.z);
    glVertex3f(to.x, to.y, to.z);

    glEnd();
    glEnable(GL_LIGHTING);
  }
  Line() :to(Vector3()), from(Vector3()) {};
};

class BBox
{
public:
  Vector3 min, max;
  BBox(Vector3 min, Vector3 max) { this->min = min; this->max = max; }

  virtual bool Contains(BBox* box) const {
    bool res = ((box->min.x >= min.x) &&
                (box->min.y >= min.y) &&
                (box->min.z >= min.z) &&
                (box->max.x <= max.x) &&
                (box->max.y <= max.y) &&
                (box->max.z <= max.z));
    return res;
  }

  bool Intersects(BBox other) const {
    return(max.x > other.min.x &&
           min.x < other.max.x &&
           max.y > other.min.y &&
           min.y < other.max.y &&
           max.z > other.min.z &&
           min.z < other.max.z);
  };
  bool Contains(Vector3 p) const {
    bool res = ((p.x >= min.x) &&
                (p.y >= min.y) &&
                (p.z >= min.z) &&
                (p.x <= max.x) &&
                (p.y <= max.y) &&
                (p.z <= max.z));
    return res;
  };
  BBox() { this->min = Vector3(); this->max = Vector3(); };
  virtual ~BBox() {}

  Vector3 BBox::center() const {

    return (max + min) / 2;
  }
};

inline float randZeroToOne()
{

  return rand() / (RAND_MAX + 1.f);
}
// drawText function used from https://rbellek.wordpress.com/category/game-engine/opengl/
inline void drawText(const char *text, int length, int x, int y) {
  glMatrixMode(GL_PROJECTION); // change the current matrix to PROJECTION
  double matrix[16]; // 16 doubles in stack memory
  glGetDoublev(GL_PROJECTION_MATRIX, matrix); // get the values from PROJECTION matrix to local variable
  glLoadIdentity(); // reset PROJECTION matrix to identity matrix
  glOrtho(0, 800, 0, 600, -5, 5); // orthographic perspective
  glMatrixMode(GL_MODELVIEW); // change current matrix to MODELVIEW matrix again
  glLoadIdentity(); // reset it to identity matrix
  glPushMatrix(); // push current state of MODELVIEW matrix to stack
  glLoadIdentity(); // reset it again. (may not be required, but it my convention)
  float sc = 3.0f;
  glScalef(sc, sc, sc);
  glRasterPos2i(x, y); // raster position in 2D
  for (int i = 0; i<length; i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]); // generation of characters in our text with 9 by 15 GLU font
  }
  glPopMatrix(); // get MODELVIEW matrix value from stack
  glMatrixMode(GL_PROJECTION); // change current matrix mode to PROJECTION
  glLoadMatrixd(matrix); // reset
  glMatrixMode(GL_MODELVIEW); // change current matrix mode to MODELVIEW
}

#endif	//VECTOR3D_H