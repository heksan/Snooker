// Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
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
double posXTrack;
double posYTrack;


double xpos = screenWidth / 2;
double ypos = screenHeight / 2;
double prevxpos = screenWidth / 2;
double prevypos = screenHeight / 2;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}



// Initial position : on +Z
glm::vec3 position = glm::vec3(30, 30, -5);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 60.0f; // 3 units / second
float mouseSpeed = 0.005f;


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
	
		//std::cout << ((double)rand() / (RAND_MAX));
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
		glm::vec3 direction(
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

		float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
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


void lookForCueStick( glm::vec3& rayOrigin, glm::vec3& rayDirection){
	glfwGetCursorPos(window, &posXTrack, &posYTrack);
	// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
	glm::vec4 lRayStart_NDC(
		((float)posXTrack / (float)screenWidth - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((float)posYTrack / (float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
		);
	glm::vec4 lRayEnd_NDC(
		((float)posXTrack / (float)screenWidth - 0.5f) * 2.0f,
		((float)posYTrack / (float)screenHeight - 0.5f) * 2.0f,
		0.0,
		1.0f
		);


	// The Projection matrix goes from Camera Space to NDC.
	// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
	glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

	// The View Matrix goes from World Space to Camera Space.
	// So inverse(ViewMatrix) goes from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

	glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayStart_world = InverseViewMatrix       * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
	glm::vec4 lRayEnd_world = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;


	// Faster way (just one inverse)
	//glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
	//glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
	//glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);


	rayOrigin = glm::vec3(lRayStart_world);
	rayDirection = glm::normalize(lRayDir_world);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS){
		checkRayIntersection(rayOrigin, rayDirection);
	}
}