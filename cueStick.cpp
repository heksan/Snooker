// Include GLEW
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


#include "cueStick.h"

GLfloat vertex_buffer_data_stick_g[] = {//2d cause lazy
	-0.6f, 0.0f, -75.0f,
	-0.6f, 0.0f, 75.0f,
	0.6f, 0.0f, 75.0f,
	-0.6f, 0.0f, -75.0f,
	0.6f, 0.0f, -75.0f,
	0.6f, 0.0f, 75.0f,
};

//floor colour
GLfloat color_buffer_data_stick_g[] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
};


CueStick::CueStick()
{

	for (int i = 0; i < 18; i++){
		vertex_buffer_data_stick[i] = vertex_buffer_data_stick_g[i];
		color_buffer_data_stick[i] = color_buffer_data_stick_g[i];
	};
	glm::mat4 matrix = glm::mat4(1.0);
	position = glm::vec3(0.0f,0.0f,displacement);
	rotation = 0.0f;

}
