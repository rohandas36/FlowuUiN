#ifndef UI_H
#define UI_H

#include <vector>
#include <math.h>
#include <GLFW/glfw3.h>

#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "Grid.hpp"

using namespace std;

void UiInit(GLFWwindow* window);
void updateState(GLFWwindow* window, Grid* fluid,
				Vector3* camerapos,Vector3* camera_z,Vector3* camera_y,
				Vector3* lightPos);


#endif