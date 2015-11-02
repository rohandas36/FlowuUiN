#ifndef VECTOR4_H
#define VECTOR4_H

#include <math.h>
#include "Vector3.hpp"


class Vector4 {
public:
	float x,y,z,w;

	Vector4();
	Vector4(float x_,float y_,float z_);
	Vector4(float x_,float y_,float z_,float w_);
	Vector4(Vector3 V,float w_);
	Vector4(const Vector4 &V);
	Vector4 operator+(const Vector4& V);
	Vector4 operator-();
	Vector4 operator-(const Vector4& V);
	float operator|(const Vector4& V);
	Vector4 operator*(const float& C);
	Vector4 operator*(const Vector4& V);
	Vector3 xyz();

	Vector4 norm();
	void print();

};

#endif