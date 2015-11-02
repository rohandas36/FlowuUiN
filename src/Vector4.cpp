#include "Vector4.hpp"
#include <iostream>
#include <string>

Vector4 Vector4::norm(){
	if(w!=0)
		return Vector4(x/w,y/w,z/w,1);
	
	return Vector4();
}
	
Vector4::Vector4(){
	x=0;
	y=0;
	z=0;
	w=0;
}
Vector4::Vector4(float x_,float y_,float z_){
	x=x_;
	y=y_;
	z=z_;
	w=1;
}

Vector4::Vector4(float x_,float y_,float z_, float w_){
	x=x_;
	y=y_;
	z=z_;
	w=w_;
}

Vector4::Vector4(Vector3 V,float w_){
	x=V.x;
	y=V.y;
	z=V.z;
	w=w_;
}

Vector4::Vector4(const Vector4 &V){
	x=V.x;
	y=V.y;
	z=V.z;
	w=V.w;
}



Vector4 Vector4::operator+(const Vector4& V){
	return Vector4(x+V.x,y+V.y,z+V.z,w+V.w);
}


Vector4 Vector4::operator-(){
	return Vector4(-x,-y,-z,-w);
}

Vector4 Vector4::operator-(const Vector4& V){
	return Vector4(x-V.x,y-V.y,z-V.z);
}

float Vector4::operator|(const Vector4& V){
	return x*V.x+y*V.y+z*V.z+w*V.w;
}

Vector4 Vector4::operator*(const float& C){
	return Vector4(x*C,y*C,z*C,w*C);
}


Vector4 Vector4::operator*(const Vector4& V){
	return Vector4(y*V.z-z*V.y,V.x*z-x*V.z,x*V.y-y*V.x,0);
}


void Vector4::print(){
	std::cout<<x<<" "<<y<<" "<<z<<" "<<w<<std::endl;
}

Vector3 Vector4::xyz(){
	return Vector3(x,y,z);
}