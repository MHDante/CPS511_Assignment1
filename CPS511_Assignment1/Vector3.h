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

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>
#include <gl/gl.h>
class Vector3
{
public:
	//constructors
	Vector3(void)	:	x(0.0f), y(0.0f), z(0.0f)
	{}

	Vector3(float newX, float newY, float newZ)	:	x(newX), y(newY), z(newZ)
	{}

	Vector3(const float * rhs)	:	x(*rhs), y(*(rhs+1)), z(*(rhs+2))
	{}

	Vector3(const Vector3 & rhs)	:	x(rhs.x), y(rhs.y), z(rhs.z)
	{}

	~Vector3() {}	//empty

	void Set(float newX, float newY, float newZ)
	{	x=newX;	y=newY;	z=newZ;	}
	
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

	
	float GetLength() const
	{	return (float)sqrt((x*x)+(y*y)+(z*z));	}
	
	float GetQuaddLength() const
	{	return (x*x)+(y*y)+(z*z);	}

	//rotations
	void RotateX(double angle);
	Vector3 GetRotatedX(double angle) const;
	void RotateY(double angle);
	Vector3 GetRotatedY(double angle) const;
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
	{	return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;}


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
	friend Vector3 operator*(float scaleFactor, const Vector3 & rhs);

	//Add, subtract etc, saving the construction of a temporary
	void Add(const Vector3 & v2, Vector3 & result)
	{
		result.x=x+v2.x;
		result.y=y+v2.y;
		result.z=z+v2.z;
	}

	void Subtract(const Vector3 & v2, Vector3 & result)
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

	//member variables
	float x;
	float y;
	float z;

  GLfloat* toGLFloat4(GLfloat w) const
  {
    return new GLfloat[4]{ x,y,z,w };
  }
  GLfloat* toGLFloat3() const
  {
    return new GLfloat[3]{ x,y,z };
  }
};

#endif	//VECTOR3D_H