#include "Vector3.hpp"
#include <iostream>
#include <string>

Vector3::Vector3(){
	x=0;
	y=0;
	z=0;
}

Vector3::Vector3(float x_,float y_,float z_){
	x=x_;
	y=y_;
	z=z_;
}

Vector3::Vector3(const Vector3 &V){
	x=V.x;
	y=V.y;
	z=V.z;
}

Vector3 Vector3::operator+(const Vector3& V){
	return Vector3(x+V.x,y+V.y,z+V.z);
}


Vector3 Vector3::operator-(){
	return Vector3(-x,-y,-z);
}

Vector3 Vector3::operator-(const Vector3& V){
	return Vector3(x-V.x,y-V.y,z-V.z);
}

float Vector3::operator|(const Vector3& V){
	return x*V.x+y*V.y+z*V.z;
}


float Vector3::mod(){
	return sqrt(x*x+y*y+z*z);
}


Vector3 Vector3::setlen(float R){
	float len=mod();
	return Vector3(R*x/len,R*y/len,R*z/len);
}


Vector3 Vector3::operator*(const float& C){
	return Vector3(x*C,y*C,z*C);
}


Vector3 Vector3::operator*(const Vector3& V){
	return Vector3(y*V.z-z*V.y,V.x*z-x*V.z,x*V.y-y*V.x);
}

void Vector3::print(){
	std::cout<<x<<" "<<y<<" "<<z<<std::endl;
}