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
const float barsNumber = 75.0f;
const float powerBarStart = 200.0f;
const float powerBarHeight = 5.0f; // height of one rect in power bar

//creates buffers and shared for power bar (2d elem of gui)
void initPowerBar2D(){
	TextShaderID = LoadShaders("TextVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
	glGenBuffers(1, &TextVertexBufferID);
	glGenBuffers(1, &TextColBufferID);
	TextUniformID = glGetUniformLocation(TextShaderID, 0);

}

//prints gui element and fills it with color based on force
void printPowerBar2D(float force){
	int redBars = round((force / 10.0f)*barsNumber);// scale up to bars num, 10.1 is max force
	std::vector<glm::vec2> barElements;
	std::vector<glm::vec3> barColors;
	for (int i = 0; i < barsNumber; i++){
		barElements.push_back(glm::vec2(1024.0f, powerBarStart+(i*powerBarHeight)));//768
		barElements.push_back(glm::vec2(1024.0f, powerBarStart + powerBarHeight + (i*powerBarHeight)));
		barElements.push_back(glm::vec2(999.0f, powerBarStart + powerBarHeight + (i*powerBarHeight)));
		barElements.push_back(glm::vec2(1024.0f, powerBarStart + (i*powerBarHeight)));
		barElements.push_back(glm::vec2(999.0f, powerBarStart+powerBarHeight + (i*powerBarHeight)));
		barElements.push_back(glm::vec2(999.0f, powerBarStart + (i*powerBarHeight)));
		if (i <= redBars){
			barColors.push_back(glm::vec3(1, 0, 0));
			barColors.push_back(glm::vec3(1, 0, 0));
			barColors.push_back(glm::vec3(1, 0, 0));
			barColors.push_back(glm::vec3(1, 0, 0));
			barColors.push_back(glm::vec3(1, 0, 0));
			barColors.push_back(glm::vec3(1, 0, 0));
		}
		else{
			barColors.push_back(glm::vec3(1, 1, 1));
			barColors.push_back(glm::vec3(1, 1, 1));
			barColors.push_back(glm::vec3(1, 1, 1));
			barColors.push_back(glm::vec3(1, 1, 1));
			barColors.push_back(glm::vec3(1, 1, 1));
			barColors.push_back(glm::vec3(1, 1, 1));
		}
	}

	glUseProgram(TextShaderID);

	glBindBuffer(GL_ARRAY_BUFFER, TextVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, barElements.size() * sizeof(glm::vec2), &barElements[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, TextColBufferID);
	glBufferData(GL_ARRAY_BUFFER, barColors.size() * sizeof(glm::vec3), &barColors[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, TextVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, TextColBufferID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, barElements.size());
	
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	barElements.clear();
	barColors.clear();
}
