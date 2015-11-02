#ifndef VECTOR3_H
#define VECTOR3_H
#include <math.h>

class Vector3 {
public:
	float x,y,z; //denote the x, y and z components of the vector 

	Vector3();
	Vector3(float x_,float y_,float z_);
	Vector3(const Vector3 &V);
	Vector3 operator+(const Vector3& V);
	float mod();
	Vector3 setlen(float R);
	Vector3 operator-();
	Vector3 operator-(const Vector3& V);
	float operator|(const Vector3& V);
	Vector3 operator*(const float& C);
	Vector3 operator*(const Vector3& V);
	void print();

};

#endif