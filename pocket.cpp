// Include GLEW
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


#include "pocket.h"

GLfloat vertex_buffer_data_pocket_g[] = {//size 3,5inch
	-4.45f, -2.6f, -4.45f,
	-4.45f, -2.6f, 4.45f,
	4.45f, -2.6f, 4.45f,
	-4.45f, -2.6f, -4.45f,
	4.45f, -2.6f, -4.45f,
	4.45f, -2.6f, 4.45f,
};

//floor colour
GLfloat color_buffer_data_pocket_g[] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
};


Pocket::Pocket(glm::vec3 pocketPosition)
{
	for (int i = 0; i < 18; i++){
		vertex_buffer_data[i] = vertex_buffer_data_pocket_g[i];
		color_buffer_data[i] = color_buffer_data_pocket_g[i];
	};
	glm::mat4 matrix = glm::mat4(1.0);
	position = pocketPosition;
}
Pocket::~Pocket()
{
}