#include "Matrix4.hpp"

Matrix4::Matrix4(){
	column[0]=Vector4(1,0,0,0);
	column[1]=Vector4(0,1,0,0);
	column[2]=Vector4(0,0,1,0);
	column[3]=Vector4(0,0,0,1);
}

Matrix4::Matrix4(Vector4 c1,Vector4 c2,Vector4 c3,Vector4 c4){
	column[0]=c1;
	column[1]=c2;
	column[2]=c3;
	column[3]=c4;
}
Matrix4::Matrix4(Vector3 z, Vector3 y){
	z=z.setlen(1);
	Vector3 x=(y*z).setlen(1);
	y=(z*x).setlen(1);
	column[0]=Vector4(x.x,y.x,z.x,0);
	column[1]=Vector4(x.y,y.y,z.y,0);
	column[2]=Vector4(x.z,y.z,z.z,0);
	column[3]=Vector4(0,0,0,1);

}

Matrix4::Matrix4(Vector3 z, Vector3 y,Matrix4::matType T){
	if(T==Matrix4::BASIS){
		z=z.setlen(1);
		Vector3 x=(y*z).setlen(1);
		y=(z*x).setlen(1);
		column[0]=Vector4(x.x,x.y,x.z,0);
		column[1]=Vector4(y.x,y.y,y.z,0);
		column[2]=Vector4(z.x,z.y,z.z,0);
		column[3]=Vector4(0,0,0,1);
	}

}

Matrix4::Matrix4(Vector3 V , Matrix4::matType T){
	if(T==Matrix4::TRANSLATE){
		column[0]=Vector4(1,0,0,0);
		column[1]=Vector4(0,1,0,0);
		column[2]=Vector4(0,0,1,0);
		column[3]=Vector4(V.x,V.y,V.z,1);
	}else if(T==Matrix4::SCALE){
		column[0]=Vector4(V.x,0,0,0);
		column[1]=Vector4(0,V.y,0,0);
		column[2]=Vector4(0,0,V.z,0);
		column[3]=Vector4(0,0,0,1);
	}else if(T==Matrix4::PROJECTION){
		column[0]=Vector4(9.0/16.0,0,0,0);
		column[1]=Vector4(0,1,0,0);
		column[2]=Vector4(0,0,(100.1)/(99.9),-1);
		column[3]=Vector4(0,0,(-(20)/(99.9)),0);
	}
	else if (T==Matrix4::ROTATION_Z){
		float theta = V.mod()*(-0.0006);
		column[0]=Vector4(cos(theta),-sin(theta),0,0);
		column[1]=Vector4(sin(theta),cos(theta),0,0);
		column[2]=Vector4(0,0,1,0);
		column[3]=Vector4(0,0,0,1);
	}
}
//fov aspect zn zf
Matrix4::Matrix4(Vector4 in, matType T){
	if(T==Matrix4::PROJECTION){
		column[0]=Vector4(1/(tan(in.x*PI/360)*in.y),0,0,0);
		column[1]=Vector4(0,1/tan(in.x*PI/360),0,0);
		column[2]=Vector4(0,0,(2*in.z*in.w)/(in.z-in.w),-1);
		column[3]=Vector4(0,0,(in.z+in.w)/(in.z-in.w),0);
	}
}


Matrix4::Matrix4(const Matrix4 &M){
	column[0]=M.column[0];
	column[1]=M.column[1];
	column[2]=M.column[2];
	column[3]=M.column[3];
}

Vector4 Matrix4::operator*(const Vector4& V){
	return (column[0]*V.x+column[1]*V.y+column[2]*V.z+column[3]*V.w);
}

Matrix4 Matrix4::operator*(const Matrix4& M){
	Matrix4 M_;
	M_.column[0]=column[0]*M.column[0].x+column[1]*M.column[0].y+column[2]*M.column[0].z+column[3]*M.column[0].w;
	M_.column[1]=column[0]*M.column[1].x+column[1]*M.column[1].y+column[2]*M.column[1].z+column[3]*M.column[1].w;
	M_.column[2]=column[0]*M.column[2].x+column[1]*M.column[2].y+column[2]*M.column[2].z+column[3]*M.column[2].w;
	M_.column[3]=column[0]*M.column[3].x+column[1]*M.column[3].y+column[2]*M.column[3].z+column[3]*M.column[3].w;
	return M_;
}

Matrix4 Matrix4::operator+(const Matrix4& M){
	Matrix4 M_;
	M_.column[0]=column[0]+M.column[0];
	M_.column[1]=column[1]+M.column[1];
	M_.column[2]=column[2]+M.column[2];
	M_.column[3]=column[3]+M.column[3];
	return M_;
}

float* Matrix4::ptr(){

	for(int i=0;i!=4;i++){
		buffer[4*i]=column[i].x;
		buffer[4*i+1]=column[i].y;
		buffer[4*i+2]=column[i].z;
		buffer[4*i+3]=column[i].w;
	}
	return buffer;
}

void Matrix4::print(){
	std::cout<<column[0].x<<" "<<column[1].x<<" "<<column[2].x<<" "<<column[3].x<<std::endl
		<<column[0].y<<" "<<column[1].y<<" "<<column[2].y<<" "<<column[3].y<<std::endl
		<<column[0].z<<" "<<column[1].z<<" "<<column[2].z<<" "<<column[3].z<<std::endl
		<<column[0].w<<" "<<column[1].w<<" "<<column[2].w<<" "<<column[3].w<<std::endl;
}