// Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>
extern GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.h"
#include "gameEngine.h"
const float screenWidth = 1024.0f;
const float screenHeight = 768.0f;
const float FoV = 45.0f;
const float speed = 45.0f;
const float mouseSpeed = 0.005f;
const float powerScaleFactor = 35.0f;

double xpos = screenWidth / 2.0f;
double ypos = screenHeight / 2.0f;
double prevxpos = screenWidth / 2.0f;
double prevypos = screenHeight / 2.0f;

glm::vec3 position = glm::vec3(100, 200, -5);
glm::vec3 direction;
float horizontalAngle = -3.14f / 2.0f;
float verticalAngle = -1.0f;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

//checks if mouse is hit in playing area or in power bar, sets force
void checkStart(bool& cueStickMoving,float& force){

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS){
		if (xpos>999.0f && screenHeight - ypos>200.0f && screenHeight-ypos<575.0f){
			force = (screenHeight - float(ypos) - 200.0f) / powerScaleFactor; 
		}
		else{
			cueStickMoving = true;
		}
	}
}

void computeCameraMatricesFromInputs(){
	//called only once
	static double lastTime = glfwGetTime();
	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS){
		glfwGetCursorPos(window, &xpos, &ypos);
		horizontalAngle += mouseSpeed * float(prevxpos - xpos);
		verticalAngle += mouseSpeed * float(prevypos - ypos);
	}
	glfwGetCursorPos(window, &xpos, &ypos);
	prevxpos = xpos;
	prevypos = ypos;

	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	glm::vec3 up = glm::cross(right, direction);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	//  Field of View, ratio, display range (a,b)
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 500.0f);
	ViewMatrix = glm::lookAt(
		position,
		position + direction,
		up
		);

	lastTime = currentTime;

}

//returns 2d point (x,z) where ray intersects with y=0 plane - not finished
glm::vec2 castRayThroughMouse(){

	float x = (2.0f * float(xpos)) / screenWidth - 1.0f;
	float y = 1.0f - (2.0f * float(ypos)) / screenHeight;
	glm::vec4 rayEnd = vec4(x, y, -1.0, 1.0);
	glm::vec4 eye = glm::inverse(ProjectionMatrix) * rayEnd;
	eye = vec4(eye.x, eye.y, -1.0, 0.0);
	glm::vec4 a = (glm::inverse(ViewMatrix) * eye);
	glm::vec3 rayDir = glm::vec3(a.x, a.y, a.z);
	rayDir = glm::normalize(rayDir);

	// go from camera with direction of ray and check if intercests with table
	glm::vec3 currentPos = position;
	for (int i = 0; i < 600; i++){
		currentPos = currentPos + rayDir;
		if (currentPos.y < 0.0){
			return glm::vec2(currentPos.x, currentPos.z);
		}
	}
	return glm::vec2(0.0f, 0.0f);
}

//right click button for placing ball back to "starting area"
void checkClick(bool& foulCommitted){

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS){
		foulCommitted = false;
	}
}