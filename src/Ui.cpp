#include "Ui.hpp"


void UiInit(GLFWwindow* window){
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwSetCursorPos(window, 1366/2, 768/2);
}


void updateState(GLFWwindow* window, Grid* fluid,
				Vector3* camerapos,Vector3* camera_z,Vector3* camera_y,
				Vector3* lightPos){

	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();

	glfwPollEvents();

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	xpos -= 1366/2;
	ypos -= 768/2;

	xpos/=100;
	ypos/=100;
	Vector3 centre(0,0,2),current(xpos,ypos,sqrt(4-xpos*xpos-ypos*ypos));
	Matrix4 rotMat;

	int state = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL );
	current.print();
	cout<<(centre*current).mod()<<endl;
	if((centre*current).mod()>0.005 ){
		Vector3 rotation_Goal=centre*current*300;
		Matrix4 basisTransform(-*camera_z,*camera_y,Matrix4::BASIS);
		rotation_Goal=(basisTransform*Vector4(rotation_Goal,0)).xyz();
		rotMat = 	Matrix4(Vector3(50,50,50),Matrix4::TRANSLATE)*
					Matrix4(rotation_Goal,Vector3(1.25,3.4,5.4),Matrix4::BASIS)*
					Matrix4(rotation_Goal ,Matrix4::ROTATION_Z)*
					Matrix4(rotation_Goal,Vector3(1.25,3.4,5.4))*
					Matrix4(Vector3(-50,-50,-50),Matrix4::TRANSLATE);
		// cout<<"inUi"<<endl;
		fluid->Model = rotMat*fluid->Model;	
	}
	glfwSetCursorPos(window, 1366/2, 768/2);
}
