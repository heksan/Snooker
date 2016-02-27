#include <vector>
#include <cstring>

// Include GLEW
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>   
#include <list>
extern bool stable;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


#include "shader.h"

///////////////////////////////////comment and refactor

GLuint TextVertexBufferID;
GLuint TextColBufferID;
GLuint TextShaderID;
GLuint TextUniformID;

void initText2D(){
	TextShaderID = LoadShaders("TextVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
	glGenBuffers(1, &TextVertexBufferID);
	glGenBuffers(1, &TextColBufferID);
	TextUniformID = glGetUniformLocation(TextShaderID, 0);

}

void printText2D(float force){

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec3> cols;
	for (int i = 0; i < 15; i++){
		vertices.push_back(glm::vec2(1024.0f, 200.0f+(i*25.0f)));//768
		vertices.push_back(glm::vec2(1024.0f, 225.0f + (i*25.0f)));
		vertices.push_back(glm::vec2(999.0f, 225.0f + (i*25.0f)));
		vertices.push_back(glm::vec2(1024.0f, 200.0f + (i*25.0f)));
		vertices.push_back(glm::vec2(999.0f, 225.0f + (i*25.0f)));
		vertices.push_back(glm::vec2(999.0f, 200.0f + (i*25.0f)));

		cols.push_back(glm::vec3(1, 1, 1));
		cols.push_back(glm::vec3(1, 1, 1));
		cols.push_back(glm::vec3(1, 1, 1));
		cols.push_back(glm::vec3(1, 1, 1));
		cols.push_back(glm::vec3(1, 1, 1));
		cols.push_back(glm::vec3(1, 1, 1));
	}

	glUseProgram(TextShaderID);

	glBindBuffer(GL_ARRAY_BUFFER, TextVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, TextColBufferID);
	glBufferData(GL_ARRAY_BUFFER, cols.size() * sizeof(glm::vec3), &cols[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, TextVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, TextColBufferID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	vertices.clear();
	cols.clear();
}
