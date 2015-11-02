#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include "Vector3.hpp"

using namespace std;


void loadOBJ(
	string path, 
	vector<Vector3> & out_vertices, 
	vector<Vector3> & out_normals
);

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif