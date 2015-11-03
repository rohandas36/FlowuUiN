#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <ctime>

#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "utils.hpp"
#include "Ui.hpp"
#include "Grid.hpp"


using namespace std;



int main(int argv, char** argc){

	GLFWwindow* window;
	stringstream in;
	in.str(string(argc[1]));
	int select;
	in>>select;

	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow( 1366, 768, "Fluid Simmulator", glfwGetPrimaryMonitor(), NULL);
	if( window == NULL ){
		fprintf( stderr ,"%s\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 

	// glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders( "shaders/default.vert", "shaders/default.frag" );

	
	glUseProgram(programID);

	GLuint MatrixID = glGetUniformLocation(programID, "PCM");
	GLuint ModelID = glGetUniformLocation(programID, "Model");
	GLuint CameraID = glGetUniformLocation(programID, "C");
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	
	Vector3 camera_z = Vector3(0,-1,0);
	Vector3 camera_y = Vector3(0,0,1);
	Vector3 cameraPos = Vector3(50,-100,50);
	Vector3 lightPos = Vector3(50,0,100);

	UiInit(window);
	Grid thisGrid;

	bool first=true;
	vector<float> fluid_vertices;
	vector<float> fluid_normals;

	do{
		
		
		vector<Vector3> Vertices,Normals;
	//**********************************************************
		// clock_t begin = clock();
		
		// cerr<<"Update Started..\n";	
		if(first && select == 0){
			thisGrid.update(select);
			// cerr<<"Update Done..\n";
			thisGrid.draw(select,&Vertices,&Normals);
			first = false;
		}
		else if (select != 0){
			thisGrid.update(select);
			fluid_vertices.clear();
			fluid_normals.clear();
			thisGrid.draw(select,&Vertices,&Normals);
		}
		// cerr<<"Triangles Done..\n";

		// clock_t end = clock();
		// float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;

		// cerr<<"FPS "<<int(1/elapsed_secs)<<endl;
		// cerr<<"Vertices "<<RendererOutput.first.size()<<endl;
		
	//**********************************************************

		if(select != 2){
			for(int i=0;i!=Vertices.size();i++){
				fluid_vertices.push_back(Vertices[i].x);
				fluid_vertices.push_back(Vertices[i].y);
				fluid_vertices.push_back(Vertices[i].z);
			}
			for(int i=0;i!=Normals.size();i++){
				fluid_normals.push_back(Normals[i].x);
				fluid_normals.push_back(Normals[i].z);
				fluid_normals.push_back(Normals[i].y);
			}

			GLuint fluid_vertexbuffer;
			glGenBuffers(1, &fluid_vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, fluid_vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, fluid_vertices.size()*sizeof(float), &fluid_vertices[0], GL_STATIC_DRAW);
			
			GLuint fluid_normalbuffer;
			glGenBuffers(1, &fluid_normalbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, fluid_normalbuffer);
			glBufferData(GL_ARRAY_BUFFER, fluid_normals.size()*sizeof(float), &fluid_normals[0], GL_STATIC_DRAW);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(programID);


			Matrix4 cameraMatrix = Matrix4(camera_z,camera_y)*Matrix4(-cameraPos,Matrix4::TRANSLATE);
			Matrix4 ProjectionMatrix = Matrix4(Vector4(60,(1366.0/768.0),10,300),Matrix4::PROJECTION);
			Matrix4 PCM = ProjectionMatrix * cameraMatrix * thisGrid.Model;

			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, PCM.ptr());
			glUniformMatrix4fv(ModelID, 1, GL_FALSE, thisGrid.Model.ptr());
			glUniformMatrix4fv(CameraID, 1, GL_FALSE, cameraMatrix.ptr());

			glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, fluid_vertexbuffer);
			glVertexAttribPointer(
				0,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);


			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, fluid_normalbuffer);
			glVertexAttribPointer(
				1,                                // attribute
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

			glDrawArrays(GL_TRIANGLES, 0, fluid_vertices.size() );

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			glDeleteBuffers(1, &fluid_vertexbuffer);
			glDeleteBuffers(1, &fluid_normalbuffer);

			glfwSwapBuffers(window);
		}
		else{

		}

		updateState(window,&thisGrid,
				&cameraPos,&camera_z,&camera_y,
				&lightPos);

	}

	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();

	return 0;
}

