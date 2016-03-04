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

GLuint powerBarVertexBufferID;
GLuint powerBarColorBufferID;
GLuint playerIndicatorVertexBufferID;
GLuint playerIndicatorColorBufferID;
GLuint Shader2DID;
GLuint Uniform2DID;
const float barsNumber = 75.0f;
const float powerBarStart = 200.0f;
const float powerBarHeight = 5.0f; // height of one rect in power bar

//creates buffers and shared for power bar (2d elem of gui)
void initPowerBar2D(){
	Shader2DID = LoadShaders("TextVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
	glGenBuffers(1, &powerBarVertexBufferID);
	glGenBuffers(1, &powerBarColorBufferID);
	Uniform2DID = glGetUniformLocation(Shader2DID, 0);

}

void initPlayerIndicator2D(){
	glGenBuffers(1, &playerIndicatorVertexBufferID);
	glGenBuffers(1, &playerIndicatorColorBufferID);
	Uniform2DID = glGetUniformLocation(Shader2DID, 0);

}


//prints gui element and fills it with color based on force
void printPowerBar2D(float force){
	int redBars = round((force / 10.48f)*barsNumber);// scale up to bars num, 10.48 is max force
	std::vector<glm::vec2> barElements;
	std::vector<glm::vec3> barColors;
	for (int i = 0; i < barsNumber; i++){
		barElements.push_back(glm::vec2(1024.0f, powerBarStart+(i*powerBarHeight)));//768
		barElements.push_back(glm::vec2(1024.0f, powerBarStart + powerBarHeight + (i*powerBarHeight)));
		barElements.push_back(glm::vec2(999.0f, powerBarStart + powerBarHeight + (i*powerBarHeight)));
		barElements.push_back(glm::vec2(1024.0f, powerBarStart + (i*powerBarHeight)));
		barElements.push_back(glm::vec2(999.0f, powerBarStart+powerBarHeight + (i*powerBarHeight)));
		barElements.push_back(glm::vec2(999.0f, powerBarStart + (i*powerBarHeight)));
		if (i < redBars){
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

	glUseProgram(Shader2DID);

	glBindBuffer(GL_ARRAY_BUFFER, powerBarVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, barElements.size() * sizeof(glm::vec2), &barElements[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, powerBarColorBufferID);
	glBufferData(GL_ARRAY_BUFFER, barColors.size() * sizeof(glm::vec3), &barColors[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, powerBarVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, powerBarColorBufferID);
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

void printPlayerIndicator2D(int id){ //refactor, elements are const
	std::vector<glm::vec2> indicatorElements;
	std::vector<glm::vec3> indicatorColors;
		indicatorElements.push_back(glm::vec2(0.0f, 768.0f));//768
		indicatorElements.push_back(glm::vec2(0.0f, 728.0f));
		indicatorElements.push_back(glm::vec2(40.0f, 768.0f));
		indicatorElements.push_back(glm::vec2(0.0f, 728.0f));
		indicatorElements.push_back(glm::vec2(40.0f, 768.0f));
		indicatorElements.push_back(glm::vec2(40.0f,728.0f));

		if (id == 1){
			indicatorColors.push_back(glm::vec3(1, 0, 0));
			indicatorColors.push_back(glm::vec3(1, 0, 0));
			indicatorColors.push_back(glm::vec3(1, 0, 0));
			indicatorColors.push_back(glm::vec3(1, 0, 0));
			indicatorColors.push_back(glm::vec3(1, 0, 0));
			indicatorColors.push_back(glm::vec3(1, 0, 0));
		}
		else{
			indicatorColors.push_back(glm::vec3(0, 1, 0));
			indicatorColors.push_back(glm::vec3(0, 1, 0));
			indicatorColors.push_back(glm::vec3(0, 1, 0));
			indicatorColors.push_back(glm::vec3(0, 1, 0));
			indicatorColors.push_back(glm::vec3(0, 1, 0));
			indicatorColors.push_back(glm::vec3(0, 1, 0));

		}
			


	glUseProgram(Shader2DID);

	glBindBuffer(GL_ARRAY_BUFFER, playerIndicatorVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, indicatorElements.size() * sizeof(glm::vec2), &indicatorElements[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, playerIndicatorColorBufferID);
	glBufferData(GL_ARRAY_BUFFER, indicatorColors.size() * sizeof(glm::vec3), &indicatorColors[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, playerIndicatorVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, playerIndicatorColorBufferID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, indicatorElements.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	indicatorElements.clear();
	indicatorColors.clear();
}