/*
    Vector class
    © Keith O'Conor 2004
    keith.oconor @ {cs.tcd.ie, gmail.com}

    modification by Fabrizio (added a method + renamed Vector3f)
*/

#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <iostream>

class Vector3f{
public:
	float x;
	float y;
	float z;

	/* 
	Vector();
	Vector(float a, float b, float c);
	Vector(Vector& copy);

	Vector& operator=(const Vector &other);
	int		operator== (const Vector &other);
	int		operator!= (const Vector &other);
	Vector	operator+(const Vector &other);
	Vector	operator-(const Vector &other);
	Vector	operator*(const float &value);
	Vector	operator/(const float &value);
	Vector& operator+=(const Vector &other);
	Vector& operator-=(const Vector &other);
	Vector& operator*=(const float& other);
	Vector& operator/=(const float& other);
	float	operator[](unsigned i);

	float	length();
	float	lengthSq();
	float	dotProduct(const Vector &other);
	Vector	crossProduct(const Vector &other);
	void	normalize();
	float	distance(const Vector &other);
	float	distanceSq(const Vector &other);
	void	set(float newX, float newY, float newZ);
	void	zero();
	*/

	  /////////////////
	 // Constructors
	/////////////////

    inline Vector3f():x(0),y(0),z(0){}
    inline Vector3f(const float a, const float b, const float c):x(a),y(b),z(c){}
    inline Vector3f(const Vector3f& copy):x(copy.x),y(copy.y),z(copy.z){}

	  //////////////
	 // Operators
	//////////////

    inline Vector3f& operator= (const Vector3f &other){
		x=other.x;y=other.y;z=other.z;
		return *this;
	}

    inline int operator== (const Vector3f &other) const{
		return (x==other.x && y==other.y && z==other.z);
	}
    inline int operator!= (const Vector3f &other) const{
		return (x!=other.x || y!=other.y || z!=other.z);
	}

    inline Vector3f operator+ (const Vector3f &other) const{
        return Vector3f(x+other.x, y+other.y, z+other.z);
	}

    inline Vector3f operator- (const Vector3f &other) const{
        return Vector3f(x-other.x, y-other.y,	z-other.z);
	}

    inline Vector3f operator* (const float &value) const{
        return Vector3f(x*value, y*value,	z*value);
	}

    inline Vector3f operator/ (const float &value) const{
        return Vector3f(x/value, y/value,	z/value);
	}

    inline Vector3f& operator+= (const Vector3f &other){
		x+=other.x;
		y+=other.y;
		z+=other.z;
		return *this;
	}
    inline Vector3f& operator-= (const Vector3f &other){
		x-=other.x;
		y-=other.y;
		z-=other.z;
		return *this;
	}

    inline Vector3f& operator*= (const float &value){
		x*=value;
		y*=value;
		z*=value;
		return *this;
	}

    inline Vector3f& operator/= (const float &value){
		x/=value;
		y/=value;
		z/=value;
		return *this;
	}

	inline float operator[] (unsigned i) const{
		switch(i){
		case 0:return x;
		case 1:return y;
		case 2:return z;
		}
	}

	  /////////////////////
	 // Other operations 
	/////////////////////

	inline float length() const{
		float len=(x*x)+(y*y)+(z*z);
		return (float)sqrt(len);
	}

	inline float lengthSq() const{
		return (x*x)+(y*y)+(z*z);
	}

    inline float dotProduct(const Vector3f &other) const{
		//this[dot]other
		return (x*other.x) + (y*other.y) + (z*other.z);
	}

    inline Vector3f crossProduct(const Vector3f &other) const{
		//(x1,y1,z1)×(x2,y2,z2) = (y1z2-y2z1, x2z1-x1z2, x1y2-x2y1). 
        return Vector3f(
			(y*other.z) - (z*other.y),
			(z*other.x) - (x*other.z),
			(x*other.y) - (y*other.x)
		);
	}

	inline void normalize(){
		float len=length();
		if(len==0)return;
		len=1.0f/len;
		x*=len;
		y*=len;
		z*=len;
	}

    inline float distance(const Vector3f &other) const{
        return (Vector3f(other.x-x,other.y-y,other.z-z)).length();
	}

    inline float distanceSq(const Vector3f &other) const{
        return (Vector3f(other.x-x,other.y-y,other.z-z)).lengthSq();
	}

	inline void set(float newX, float newY, float newZ){
		x=newX;y=newY;z=newZ;
	}

	inline void zero(){
		x=y=z=0;
	}

	//rotation (from openframework)
    inline Vector3f getRotated(float ax, float ay, float az) const {
		 float a = (float)cos(PI/180*(ax));
		 float b = (float)sin(PI/180*(ax));
		 float c = (float)cos(PI/180*(ay));
		 float d = (float)sin(PI/180*(ay));
		 float e = (float)cos(PI/180*(az));
		 float f = (float)sin(PI/180*(az));
 
		 float nx = c * e * x - c * f * y + d * z;
		 float ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
		 float nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
 
         return Vector3f( nx, ny, nz );
	}

    //Fabrizio (just silly method to use the vector in functions like glvertex3fv()
    float* fv(){
        float* result=new float[3];
        result[0]=this->x;
        result[1]=this->y;
        result[2]=this->z;
        return result;
    }

};

typedef Vector3f Vertex;

const Vector3f vZero=Vector3f(0,0,0);

  /////////////////////////////
 // Global stream operators 
//////////////////////////////
inline std::ostream& operator<<(std::ostream &str, const Vector3f &v){
	str<<v.x<<", "<<v.y<<", "<<v.z;
	return str;
}




#endif //VECTOR_H
