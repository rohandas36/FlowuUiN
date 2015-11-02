#include "Ui.hpp"

double xlast=1366/2,ylast=768/2;

void UiInit(GLFWwindow* window){
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwSetCursorPos(window, 1366/2, 768/2);
}


void updateState(GLFWwindow* window, Grid* fluid,
				Vector3* camerapos,Vector3* camera_z,Vector3* camera_y,
				Vector3* lightPos){
	
	glfwPollEvents();

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	double temp;
	temp = xpos;
	xpos-=xlast;
	xlast=temp;
	temp=ypos;
	ypos-=ylast;
	ylast=temp;

	xpos/=1000;
	ypos/=1000;
	Vector3 centre(0,0,2),current(xpos,ypos,sqrt(4-xpos*xpos-ypos*ypos));
	Matrix4 rotMat;

	int state = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL );
	
	if((centre*current).mod()>0.005 && state == GLFW_PRESS){
		Vector3 rotation_Goal=centre*current;
		Matrix4 basisTransform1(-*camera_z,*camera_y,Matrix4::BASIS);
		Matrix4 basisTransform2(-*camera_z,-*camera_y,Matrix4::BASIS);
		rotation_Goal=(basisTransform1*Vector4(rotation_Goal,0)).xyz();
		rotMat = Matrix4(rotation_Goal,Vector3(1.25,3.4,5.4),Matrix4::BASIS)*
					Matrix4(rotation_Goal ,Matrix4::ROTATION_Z)*
					Matrix4(rotation_Goal,Vector3(1.25,3.4,5.4));
		*camera_z = (rotMat*(Vector4(*camera_z,0))).xyz().setlen(1);	
		*camera_y = (rotMat*(Vector4(*camera_y,0))).xyz().setlen(1);	
	}
}
