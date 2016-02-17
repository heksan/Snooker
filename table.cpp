// Include GLEW
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


#include "table.h"

GLfloat vertex_buffer_data_table_g[] = {
	-89.0f, -2.6f, -178.5f,
	-89.0f, -2.6f, 178.5f,
	89.0f, -2.6f, 178.5f,
	-89.0f, -2.6f, -178.5f,
	89.0f, -2.6f, -178.5f,
	89.0f, -2.6f, 178.5f,
};

//floor colour
GLfloat color_buffer_data_table_g[] = {
	0.4f, 1.0f, 0.4f,
	0.4f, 1.0f, 0.4f,
	0.4f, 1.0f, 0.4f,
	0.4f, 1.0f, 0.4f,
	0.4f, 1.0f, 0.4f,
	0.4f, 1.0f, 0.4f,
};


Table::Table()
{
	
	for (int i = 0; i < 18; i++){
		vertex_buffer_data_table[i] = vertex_buffer_data_table_g[i];
		color_buffer_data_table[i] = color_buffer_data_table_g[i];
	};
	glm::mat4 matrix = glm::mat4(1.0);

}



