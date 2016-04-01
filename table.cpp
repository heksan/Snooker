// Include GLEW
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


#include "table.h"


const float ballRadius = 2.6f;
const float pocketRadius = 3.5f;
const float tableWidth_var = 89.0f;
const float tableLength_var = 178.5f;

GLfloat vertexBufferDataTable_g[] = {//delete this g
	-89.0f, -2.6f, -178.5f,
	-89.0f, -2.6f, 178.5f,
	89.0f, -2.6f, 178.5f,
	-89.0f, -2.6f, -178.5f,
	89.0f, -2.6f, -178.5f,
	89.0f, -2.6f, 178.5f,
};

GLfloat uv_buffer_data_table_g[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f, 
	1.0f, 0.0f,
	1.0f, 1.0f,
};


Table::Table()
{
	
	for (int i = 0; i < 18; i++){
		vertexBufferDataTable[i] = vertexBufferDataTable_g[i];
	};
	for (int i = 0; i < 12; i++){
		uvBufferDataTable[i] = uv_buffer_data_table_g[i];
	};
	glm::mat4 matrix = glm::mat4(1.0);

	insertShortSide(tableWidth_var, -tableLength_var, vertexBufferDataFrame,colorBufferDataFrame);
	//mirror image xaxis
	std::vector<glm::vec3> copyVec(vertexBufferDataFrame);
	for (std::vector<glm::vec3>::iterator vertex = copyVec.begin(); vertex != copyVec.end(); vertex++){
		vertexBufferDataFrame.push_back(glm::vec3(-*&vertex->x, *&vertex->y, *&vertex->z));
	}
	std::vector<glm::vec3> copyVec2(colorBufferDataFrame);
	for (std::vector<glm::vec3>::iterator col = copyVec2.begin(); col != copyVec2.end(); col++){
		colorBufferDataFrame.push_back(*col);
	}

	copyVec.clear();
	copyVec2.clear();

	//mirror image yaxis
	std::vector<glm::vec3> copyVec3(vertexBufferDataFrame);
	for (std::vector<glm::vec3>::iterator vertex = copyVec3.begin(); vertex != copyVec3.end(); vertex++){
		vertexBufferDataFrame.push_back(glm::vec3(*&vertex->x, *&vertex->y, -*&vertex->z));
	}
	std::vector<glm::vec3> copyVec4(colorBufferDataFrame);
	for (std::vector<glm::vec3>::iterator col = copyVec4.begin(); col != copyVec4.end(); col++){
		colorBufferDataFrame.push_back(*col);
	}
	//vector1.insert( vector1.end(), vector2.begin(), vector2.end() );
	copyVec3.clear();
	copyVec4.clear();
}

void insertShortSide(float tableWidth, float tableLength, std::vector<glm::vec3>& vertexBufferDataFrame, std::vector<glm::vec3>& colorBufferDataFrame){
	
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, -2.6f, tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, -2.6f, tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, 2.6f, tableLength));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, -2.6f, tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, 2.6f, tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, tableLength));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, 2.6f, tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, tableLength - (2 * ballRadius)));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, 2.6f, tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, tableLength - (2 * ballRadius)));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, 2.6f, tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, tableLength - (2 * ballRadius)));
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, 2.6f, tableLength - (2 * ballRadius)));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
//corner1
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, tableLength - (2 * ballRadius)));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth, 2.6f, tableLength- (2 * ballRadius)));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth, 2.6f, tableLength - (2 * ballRadius)));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth + (2 * ballRadius), 2.6f, tableLength - (2 * ballRadius)));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth + (2 * ballRadius), 2.6f, tableLength));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth , 2.6f, tableLength + (2 * ballRadius)));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth + (2 * ballRadius), 2.6f, tableLength));//
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth + (2 * ballRadius), 2.6f, tableLength + (2 * ballRadius)));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));

	//side1
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth, -2.6f, tableLength+pocketRadius));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth , -2.6f, -pocketRadius));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth, 2.6f, tableLength + pocketRadius));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth, 2.6f, tableLength + pocketRadius));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth, 2.6f,  - pocketRadius));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth, -2.6f,  - pocketRadius));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth, 2.6f, tableLength + pocketRadius));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth, 2.6f, -pocketRadius));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth + 2*pocketRadius, 2.6f, tableLength + pocketRadius));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth, 2.6f, - pocketRadius));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth + 2*pocketRadius, 2.6f, -pocketRadius));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth + 2*pocketRadius, 2.6f, tableLength + pocketRadius));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));






	/*vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, -2.6f, -tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, -2.6f, -tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, 2.6f, -tableLength));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, -2.6f, -tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, 2.6f, -tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, -tableLength));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	colorBufferDataFrame.push_back(glm::vec3(0, 1, 0));
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, 2.6f, -tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, -tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, -tableLength + (2 * ballRadius)));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, 2.6f, -tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, -tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, -tableLength + (2 * ballRadius)));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, 2.6f, -tableLength));
	vertexBufferDataFrame.push_back(glm::vec3(tableWidth - pocketRadius, 2.6f, -tableLength + (2 * ballRadius)));
	vertexBufferDataFrame.push_back(glm::vec3(-tableWidth + pocketRadius, 2.6f, -tableLength + (2 * ballRadius)));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));
	colorBufferDataFrame.push_back(glm::vec3(0.8,0.2,0.2));*/
}

