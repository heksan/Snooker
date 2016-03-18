// Include GLEW
#define _USE_MATH_DEFINES
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "pocket.h"
#include <vector>
#include <math.h>




Pocket::Pocket(glm::vec3 pocketPosition)
{

	for (float i = 0.0f; i <= 360.0f; i += 30.0f){
		vertexBufferData.push_back(glm::vec3(radius*sin(i * M_PI / 180.0f), -2.55f, radius*cos(i* M_PI / 180.0f)));
		vertexBufferData.push_back(glm::vec3(radius * sin((i + 30.0f)  * M_PI / 180.0f), -2.55f, radius * cos((i + 30.0f)* M_PI / 180.0f)));
		vertexBufferData.push_back(glm::vec3(0.0f,-2.55f,0.0f));
		colorBufferData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		colorBufferData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		colorBufferData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	}



	glm::mat4 matrix = glm::mat4(1.0);
	position = pocketPosition;
}
Pocket::~Pocket()
{
}