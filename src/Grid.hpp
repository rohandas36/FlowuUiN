#ifndef PHYS_GRID_HPP
#define PHYS_GRID_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "constants.hpp"
#include "Vector3.hpp"
#include "Matrix4.hpp"

using namespace std;

// PARTICLE
//***************************************

class Particle{
public:
	
	Vector3 position,
		velocity,
		acceleration;
};

//FIELDPOINT
//****************************************

class fieldPoint{
public:
	//frame number to avoid reinitialization of sample grid
	int frameNum;
	//scaler value at thet point
	float scaler;
	Vector3 gradiant,
			position;

	fieldPoint();
	void clear(int __frameNum);
};

//TRIPLET
//****************************************

class Triplet{
public:
	int x,y,z;
	Triplet(){
		x=0;
		y=0;
		z=0;
	}
	Triplet(int __x,int __y,int __z){
		x=__x;
		y=__y;
		z=__z;
	}
};

// GRID
//****************************************


class Grid{

private:

	//frame number to avoid reinitialization of sample grid
	int frameNum;
	//Particles
	vector<Particle> particles;
	//set of particles per grid
	// dimension : GRID_X GRID_Y GRID_Z
	set <Particle*> ***space_grid;
	// set of field points 
	//dimenssion : GRID_X+1*QUALITY GRID_Y+1*QUALITY GRID_Z+1*QUALITY
	fieldPoint*** sample_grid;

	//list of sample grid elements with cube  int gradiant region
	vector<Triplet> toRender,RenderProfile,CollissionProfile;

	// Relative tetrahedron vertices
	vector<Triplet> Tetrahedrons ;

	
public:
	
	//modelview matrix (lagacy purposes)
	Matrix4 Model;

	Grid();
	// updates both space_grid and sample_grid
	// space_grid : get particle neighbours using collisionProfile()
	// 				field points using renderProfile()
	// 2pass 
	// 1st pass : particle update position and (sample and space) Grid
	// 2nd pass : update velocity acceleration using interact()
	void update();
	//iterate over whole sample_grid and return triangles and normals
	pair<vector<Vector3>,vector<Vector3> > draw();
	//interpolate between two fieldpoints one higher and the other lowerthan the thresh hold 
	//return position and gradiant at that point
	pair<Vector3,Vector3> interpolate(fieldPoint* P1,fieldPoint* P2);
	//update the effect particles acc based on the cause
	void interact(Particle* cause,Particle* effect);
	//return particles within COLL_SCAN_RADIUS of position (in scaled space)
	vector<Particle*> collisionProfile(Vector3 position);
	//return fieldpoints within REND_SCAN_RADIUS of position (also in scaled space)
	vector<Triplet> renderProfile(Vector3 position);
	//trangularize index sample cube and push triangle in Vertices and Normals
	void triangularize(Triplet index, vector<Vector3>* Vertices, vector<Vector3>* Normals);

	pair<float,float> RenderField(float r);

};

#endif

