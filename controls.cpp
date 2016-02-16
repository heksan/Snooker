// Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>
extern bool stable;
extern GLFWwindow* window;  //fix this hack

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.h"
#include "ball.h"
#include "gameEngine.h"
double screenWidth = 1024;
double screenHeight = 768;
double track;
double posYTrack;


double xpos = screenWidth / 2;
double ypos = screenHeight / 2;
double prevxpos = screenWidth / 2;
double prevypos = screenHeight / 2;

glm::vec3 position = glm::vec3(100, 200, -5);
glm::vec3 direction;
float horizontalAngle = -3.14f/2.0f;
float verticalAngle = -1.0f;
float initialFoV = 45.0f;
float speed = 45.0f; // 60 units / second
float mouseSpeed = 0.005f;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}






glm::vec3 computeMovFromInput(){//no need for args
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS){
		stable = false;
		return position;
	}
	else{
		return position;
	}

}

void computeCameraMatricesFromInputs(){
		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		// to lock mouse in center
		//horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
		//verticalAngle += mouseSpeed * float(768 / 2 - ypos);
		//glfwSetCursorPos(window, 1024 / 2, 768 / 2);
		
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS){
			glfwGetCursorPos(window, &xpos, &ypos);
			horizontalAngle += mouseSpeed * float(prevxpos - xpos);
			verticalAngle += mouseSpeed * float(prevypos - ypos);
		}
		glfwGetCursorPos(window, &xpos, &ypos);
		prevxpos = xpos;
		prevypos = ypos;

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction = glm::vec3 (
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
			);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
			);

		// Up vector
		glm::vec3 up = glm::cross(right, direction);

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
			position += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			position -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
			position += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			position -= right * deltaTime * speed;
		}

		float FoV = initialFoV;

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 500 units
		ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 500.0f);
		// Camera matrix
		ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
			);

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;

}

//returns 2d point (x,z) where ray intersects with y=0 plane
void castRay(){
	
	float x = (2.0f * xpos) / screenWidth-1.0;
	float y = 1.0f-(2.0f * ypos) / screenHeight;
	
	glm::vec4 rayEnd = vec4(x,y, -1.0, 1.0);	
	glm::vec4 eye = glm::inverse(ProjectionMatrix) * rayEnd;
	
	eye = vec4(eye.x, eye.y, -1.0, 0.0);
	glm::vec4 a = (glm::inverse(ViewMatrix) * eye);
	glm::vec3 rayDir = glm::vec3(a.x, a.y, a.z);
	rayDir = glm::normalize(rayDir);

	//std::cout << "camera direction :" << direction.x << "," << direction.y << "," << direction.z << "\n";
	//std::cout << "length :" << glm::length(direction) << "\n";
	//std::cout << "camera+ mouse direction :" << rayDir.x << "," << rayDir.y << "," << rayDir.z << "\n";
	//std::cout << "length :" << glm::length(ray_wor) << "\n";
	glm::vec3 currentPos = position;
	for (int i = 0; i < 300; i++){
		currentPos = currentPos + rayDir;
		if (currentPos.y < 0.0){
			//std::cout << "plane hit in " << currentPos.x << "," << currentPos.z << "\n";
			break;
		}
	}
}