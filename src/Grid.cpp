#include "Grid.hpp"

//PARTICLE
//**********************************************


// FIELD POINT
//**********************************************

void fieldPoint::clear(){
	scaler=0;
	gradiant = Vector3(0,0,0);
}

//GRID
//**********************************************

Grid::Grid(){

}

// updates both space_grid and sample_grid
// space_grid : get particle neighbours using collisionProfile()
// 				field points using renderProfile()
// 2pass 
// 1st pass : particle update position and (sample and space) Grid
// 2nd pass : update velocity acceleration using interact()
void Grid::update(){

}

//iterate over whole sample_grid and return triangles and normals
pair<vector<Vector3>,vector<Vector3> > Grid::draw(){

}

//interpolate between two fieldpoints one higher and the other lowerthan the thresh hold 
//return position and gradiant at that point
pair<Vector3,Vector3> Grid::interpolate(fieldPoint* P1,fieldPoint* P2){

}

//update the effect particles acc based on the cause
void Grid::interact(Particle* cause,Particle* effect){

}

//return particles within COLL_SCAN_RADIUS of position (in scaled space)
vector<Particle*> Grid::collisionProfile(Vector3 position){

}

//return fieldpoints within REND_SCAN_RADIUS of position (also in scaled space)
vector<fieldPoint*> Grid::renderProfile(Vector3 position){

}