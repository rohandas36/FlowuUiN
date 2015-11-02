#ifndef MATRIX4_H
#define MATRIX4_H

#include <vector>
#include <math.h>
#include <iostream>
#include "Vector4.hpp"
#include "Vector3.hpp"

#define PI 3.14159265

using namespace std;

class Matrix4{
public:
	enum matType { TRANSLATE, SCALE, PROJECTION, BASIS, ROTATION_Z};
	Vector4 column[4];
	float buffer[16];
	Matrix4();
	Matrix4(Vector4 c1,Vector4 c2,Vector4 c3,Vector4 c4);
	Matrix4(Vector3 z, Vector3 y);
	Matrix4(Vector3 z, Vector3 y,matType T);
	Matrix4(Vector4 in, matType T);
	Matrix4(Vector3 V , matType T);
	Matrix4(const Matrix4 &M);
	Vector4 operator*(const Vector4& V);
	Matrix4 operator*(const Matrix4& M);
	Matrix4 operator+(const Matrix4& M);
	float* ptr();
	void print();
};

#endif