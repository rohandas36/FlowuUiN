#include "Grid.hpp"

//PARTICLE
//**********************************************


// FIELD POINT
//**********************************************

fieldPoint::fieldPoint(){
	frameNum = 0;
	clear(0);
}

void fieldPoint::clear(int __frameNum){
	scaler=0;
	gradiant = Vector3(0,0,0);
	frameNum = __frameNum;
}

//GRID
//**********************************************

struct compare {
  bool operator() (const Triplet& lhs, const Triplet& rhs) const
  {
  	if(lhs.x==rhs.x){
  		if(lhs.y==rhs.y){
  			return (lhs.z<rhs.z);
  		}
  		else
  			return (lhs.y<rhs.y);
  	}
  	else
  		return (lhs.x<rhs.x);
  }
};

Grid::Grid(){
	frameNum=0;
	Triplet Temp[] = {Triplet(0,0,0),Triplet(1,1,1),Triplet(1,0,0),Triplet(1,0,1),
						Triplet(0,0,0),Triplet(1,1,1),Triplet(1,0,1),Triplet(0,0,1),
						Triplet(0,0,0),Triplet(1,1,1),Triplet(0,0,1),Triplet(0,1,1),
						Triplet(0,0,0),Triplet(1,1,1),Triplet(0,1,1),Triplet(0,1,0),
						Triplet(0,0,0),Triplet(1,1,1),Triplet(0,1,0),Triplet(1,1,0),
						Triplet(0,0,0),Triplet(1,1,1),Triplet(1,1,0),Triplet(1,0,0)
					};
	Tetrahedrons = vector<Triplet>(Temp,Temp+24);

	space_grid = new set<Particle*>**[GRID_X];
	for(int i=0;i!=GRID_X;i++){
		space_grid[i] = new set<Particle*>*[GRID_Y];
		for(int j=0;j!=GRID_Y;j++){
			space_grid[i][j] = new set<Particle*>[GRID_Z];
		}
	}

	sample_grid = new fieldPoint**[(GRID_X+1)*QUALITY];
	for(int i=0;i!=(GRID_X+1)*QUALITY;i++){
		sample_grid[i] = new fieldPoint*[(GRID_Y+1)*QUALITY];
		for(int j=0;j!=(GRID_Y+1)*QUALITY;j++){
			sample_grid[i][j] = new fieldPoint[(GRID_Z+1)*QUALITY];
			for(int k=0;k!=(GRID_Z+1)*QUALITY;k++){
				sample_grid[i][j][k].position = Vector3(float(i),float(j),float(k))*(SIDE_LENGTH/float(QUALITY));
			}
		}
	}

	float MAX_GRID_RADIUS = (REND_SCAN_RADIUS*float(QUALITY)/SIDE_LENGTH) +1;
	map<Triplet,bool,compare> isInside;
	for(int x= -int(MAX_GRID_RADIUS);x!=int(MAX_GRID_RADIUS)+1;x++){
		for(int y= -int(MAX_GRID_RADIUS);y!=int(MAX_GRID_RADIUS)+1;y++){
			for(int z= -int(MAX_GRID_RADIUS);z!=int(MAX_GRID_RADIUS)+1;z++){
				if(Vector3(float(x),float(y),float(z)).mod() <= MAX_GRID_RADIUS){
					isInside[Triplet(x,y,z)]=true;
					isInside[Triplet(x-1,y-1,z-1)]=true;
				}
			}
		}
	}
	for(map<Triplet,bool>::iterator itr=isInside.begin(); itr!=isInside.end();itr++){
		if(itr->second){
			RenderProfile.push_back(itr->first);
		}
	}


	MAX_GRID_RADIUS = (COLL_SCAN_RADIUS/SIDE_LENGTH) +1;
	isInside.clear();
	for(int x= -int(MAX_GRID_RADIUS);x!=int(MAX_GRID_RADIUS)+1;x++){
		for(int y= -int(MAX_GRID_RADIUS);y!=int(MAX_GRID_RADIUS)+1;y++){
			for(int z= -int(MAX_GRID_RADIUS);z!=int(MAX_GRID_RADIUS)+1;z++){
				if(Vector3(float(x),float(y),float(z)).mod() <= MAX_GRID_RADIUS){
					isInside[Triplet(x,y,z)]=true;
					isInside[Triplet(x-1,y-1,z-1)]=true;
				}
			}
		}
	}
	for(map<Triplet,bool>::iterator itr=isInside.begin(); itr!=isInside.end();itr++){
		if(itr->second){
			CollissionProfile.push_back(itr->first);
		}
	}

	for(int i=0;i!=NUM_PARTICLE;i++){
		int X = rand()%(GRID_X-1),
			Y = rand()%(GRID_Y-1),
			Z = rand()%(GRID_Z-1);

		Particle temp;
		temp.position = Vector3((float(X)+float(rand()%10000)/10000.0)*SIDE_LENGTH,
								(float(Y)+float(rand()%10000)/10000.0)*SIDE_LENGTH,
								(float(Z)+float(rand()%10000)/10000.0)*SIDE_LENGTH);
		particles.push_back(temp);
		space_grid[X][Y][Z].insert(&particles[particles.size()-1]);
	}
}

// updates both space_grid and sample_grid
// space_grid : get particle neighbours using collisionProfile()
// 				field points using renderProfile()
// 2pass 
// 1st pass : particle update position and (sample and space) Grid
// 2nd pass : update velocity acceleration using interact()
void Grid::update(){


	for(int i=0;i!=particles.size();i++){
		// cerr<<"Particle "<<i<<endl;
		vector<Triplet> renderStencil = renderProfile(particles[i].position);
		for(int j=0;j!=renderStencil.size();j++){
			fieldPoint* sample = &sample_grid[renderStencil[j].x][renderStencil[j].y][renderStencil[j].z];
			// cout<<sample->frameNum<<endl;
			if(sample->frameNum!=frameNum)
				sample->clear(frameNum);

			pair<float,float> field = RenderField((sample->position-particles[i].position).mod());
			sample->scaler += field.first;
			sample->gradiant = sample->gradiant+(sample->position-particles[i].position).setlen(field.second);
		}

		for(int j=0;j!=renderStencil.size();j++){
			int inside=0;
			if(renderStencil[j].x < (GRID_X+1)*QUALITY-1 && renderStencil[j].y < (GRID_Y+1)*QUALITY-1 && renderStencil[j].z < (GRID_Z+1)*QUALITY-1 ){
				for(int k=0;k!=8;k++){
					Triplet diff(renderStencil[j].x + (k%2),renderStencil[j].y + ((k/2)%2),renderStencil[j].z + ((k/4)%2));
					if(sample_grid[diff.x][diff.y][diff.z].scaler>THRESHOLD)
						inside++;
				}
			}
			if(inside!=0&&inside!=8){
				toRender.push_back(renderStencil[j]);
			}
		}
	}
	frameNum=(frameNum+1)%FRAME_WINDOW;
}

//iterate over whole sample_grid and return triangles and normals
void Grid::triangularize(Triplet index, vector<Vector3>* Vertices, vector<Vector3>* Normals){
	for(int TetNo = 0; TetNo!=6 ; TetNo++){
		vector<fieldPoint*> Inside,Outside;
		for(int j=0;j!=4;j++){
			Triplet sample = Tetrahedrons[TetNo*4+j];
			sample.x += index.x;
			sample.y += index.y;
			sample.z += index.z;
			if(sample_grid[sample.x][sample.y][sample.z].scaler > THRESHOLD)
				Inside.push_back(&sample_grid[sample.x][sample.y][sample.z]);
			else
				Outside.push_back(&sample_grid[sample.x][sample.y][sample.z]);
		}
		if(Outside.size()==1){
			swap(Inside,Outside);
		}
		if(Outside.size()==2){
			pair<Vector3,Vector3> 	diag0 = interpolate(Inside[0],Outside[0]),
									diag1 = interpolate(Inside[0],Outside[1]),
									diag2 = interpolate(Inside[1],Outside[0]),
									diag3 = interpolate(Inside[1],Outside[1]);
			Vertices->push_back(diag0.first);
			Normals->push_back(diag0.second);
			Vertices->push_back(diag3.first);
			Normals->push_back(diag3.second);
			Vertices->push_back(diag1.first);
			Normals->push_back(diag1.second);
			Vertices->push_back(diag0.first);
			Normals->push_back(diag0.second);
			Vertices->push_back(diag3.first);
			Normals->push_back(diag3.second);
			Vertices->push_back(diag2.first);
			Normals->push_back(diag2.second);
		}
		if(Outside.size()==3){
			for(int i=0;i!=3;i++){
				pair<Vector3,Vector3> vertex = interpolate(Inside[0],Outside[i]);
				Vertices->push_back(vertex.first);
				Normals->push_back(vertex.second);
			}
		}
	}
}

pair<vector<Vector3>,vector<Vector3> > Grid::draw(){
	vector<Vector3> Vertices,Normals;
	map<Triplet,bool,compare> isDone;
	for(int i=0;i!=toRender.size();i++){
		if(toRender[i].x < (GRID_X+1)*QUALITY-1 && toRender[i].y < (GRID_Y+1)*QUALITY-1 && toRender[i].z < (GRID_Z+1)*QUALITY-1 
			&& toRender[i].x >= 0 && toRender[i].y >= 0 && toRender[i].z >= 0 && !isDone[toRender[i]])
			triangularize(toRender[i],&Vertices,&Normals);
		isDone[toRender[i]] = true;
	}
	return make_pair(Vertices,Normals);
}
//scaler and negation of gradiant  assuming spherically symmetric field
pair<float,float> Grid::RenderField(float r){
	float pi = 3.14159265359;
	if(r<CUTOFF_RADIUS)
		return make_pair(0.5*(1+cos(pi*r/CUTOFF_RADIUS)),(0.5*pi/CUTOFF_RADIUS)*sin(pi*r/CUTOFF_RADIUS));
	return make_pair(0.0,0.0);
}
//interpolate between two fieldpoints one higher and the other lowerthan the thresh hold 
//return position and gradiant at that point
pair<Vector3,Vector3> Grid::interpolate(fieldPoint* P1,fieldPoint* P2){
	
	// if(P1->frameNum!=frameNum)
	// 	P1->clear(frameNum);
	// if(P2->frameNum!=frameNum)
	// 	P2->clear(frameNum);

	float alpha = (THRESHOLD-P2->scaler)/(P1->scaler-P2->scaler);
	// cout<<P2->scaler<<endl;
	return make_pair(P1->position*alpha+P2->position*(1.0 - alpha),P1->gradiant*alpha + P2->gradiant*(1.0 - alpha));
}

//update the effect particles acc based on the cause
void Grid::interact(Particle* cause,Particle* effect){

}

//return particles within COLL_SCAN_RADIUS of position (in scaled space)
vector<Particle*> Grid::collisionProfile(Vector3 position){
	vector<Particle*> coll;
	Vector3 GridSpace = position*(1/SIDE_LENGTH);
	Triplet Index(int(GridSpace.x),int(GridSpace.y),int(GridSpace.z));

	for(int i=0;i!=CollissionProfile.size();i++){

		Triplet Temp(Index.x+CollissionProfile[i].x, Index.y+CollissionProfile[i].y, Index.z+CollissionProfile[i].z); 
		if(Temp.x < GRID_X && Temp.y < GRID_Y && Temp.z < GRID_Z 
			&& Temp.x >= 0 && Temp.y >= 0 && Temp.z >= 0)	
			for(set<Particle*>::iterator itr = space_grid[Temp.x][Temp.y][Temp.z].begin(); 
				itr != space_grid[Temp.x][Temp.y][Temp.z].end(); 
				itr++)
				coll.push_back(*itr);
	}
	return coll;
}

//return fieldpoints within REND_SCAN_RADIUS of position (also in scaled space)
vector<Triplet> Grid::renderProfile(Vector3 position){
	vector<Triplet> ans;
	Vector3 GridSpace = position*(float(QUALITY)/SIDE_LENGTH);
	Triplet Index(int(GridSpace.x),int(GridSpace.y),int(GridSpace.z));

	for(int i=0;i!=RenderProfile.size();i++){
		Triplet Temp(Index.x+RenderProfile[i].x, Index.y+RenderProfile[i].y, Index.z+RenderProfile[i].z);
		if(Temp.x < (GRID_X+1)*QUALITY && Temp.y < (GRID_Y+1)*QUALITY && Temp.z < (GRID_Z+1)*QUALITY 
			&& Temp.x >= 0 && Temp.y >= 0 && Temp.z >= 0)
			ans.push_back(Temp);
	}
	return ans;

}