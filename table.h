#ifndef TABLE_H
#define TABLE_H

#include<vector>




class Table
{

public:
	GLfloat vertexBufferDataTable[18];
	std::vector<glm::vec3> vertexBufferDataFrame;
	std::vector<glm::vec3> colorBufferDataFrame;
	GLfloat uvBufferDataTable[12];
	GLuint vertexbufferTable;
	GLuint colorbufferTable;
	GLuint vertexbufferFrame;
	GLuint colorbufferFrame;
	GLuint uvbufferTable;
	glm::mat4 matrix;
	glm::mat4 MVP;
	Table();

};

void insertShortSide(float tableWidth,float tableLength, std::vector<glm::vec3>& vertexBufferDataFrame, std::vector<glm::vec3>& colorBufferDataFrame);

#endif