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

#include "graphicsEngine.h"
#include "ball.h" 
#include "table.h"



void createBuffer(Table& table){

	glGenBuffers(1, &table.vertexbufferTable);
	glBindBuffer(GL_ARRAY_BUFFER, table.vertexbufferTable);
	glBufferData(GL_ARRAY_BUFFER, sizeof(table.vertex_buffer_data_table), table.vertex_buffer_data_table, GL_STATIC_DRAW);

	glGenBuffers(1, &table.colorbufferTable);
	glBindBuffer(GL_ARRAY_BUFFER, table.colorbufferTable);
	glBufferData(GL_ARRAY_BUFFER, sizeof(table.color_buffer_data_table), table.color_buffer_data_table, GL_STATIC_DRAW);
}

void createBuffer(CueStick& cueStick){

	glGenBuffers(1, &cueStick.vertexbufferStick);
	glBindBuffer(GL_ARRAY_BUFFER, cueStick.vertexbufferStick);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cueStick.vertex_buffer_data_stick), cueStick.vertex_buffer_data_stick, GL_STATIC_DRAW);

	glGenBuffers(1, &cueStick.colorbufferStick);
	glBindBuffer(GL_ARRAY_BUFFER, cueStick.colorbufferStick);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cueStick.color_buffer_data_stick), cueStick.color_buffer_data_stick, GL_STATIC_DRAW);
}

void createBuffer(Ball& ball){
	glGenBuffers(1, &ball.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ball.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ball.vertex_buffer_data), ball.vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &ball.colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ball.colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ball.color_buffer_data), ball.color_buffer_data, GL_STATIC_DRAW);



}

void createBuffers(std::list<Ball*> listOfBalls){
	for (std::list<Ball*>::iterator currentBall = listOfBalls.begin(); currentBall != listOfBalls.end(); currentBall++){

		glGenBuffers(1, &(*currentBall)->vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, (*currentBall)->vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof((*currentBall)->vertex_buffer_data), (*currentBall)->vertex_buffer_data, GL_STATIC_DRAW);

		glGenBuffers(1, &(*currentBall)->colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, (*currentBall)->colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof((*currentBall)->color_buffer_data), (*currentBall)->color_buffer_data, GL_STATIC_DRAW);


	}

}

void drawBall(Ball ball, GLuint MatrixID, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix){


	ball.MVP = ProjectionMatrix * ViewMatrix * ball.matrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &ball.MVP[0][0]);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, ball.vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, ball.colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, ball.noVertices / 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


void drawBalls(std::list<Ball*> listOfBalls, GLuint MatrixID, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix){
	for (std::list<Ball*>::iterator currentBall = listOfBalls.begin(); currentBall != listOfBalls.end(); currentBall++){
		(*currentBall)->MVP = ProjectionMatrix * ViewMatrix * (*currentBall)->matrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(*currentBall)->MVP[0][0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, (*currentBall)->vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, (*currentBall)->colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, (*currentBall)->noVertices / 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

}

void drawTable(Table table, GLuint MatrixID, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix){


	table.MVP = ProjectionMatrix * ViewMatrix * table.matrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &table.MVP[0][0]);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, table.vertexbufferTable);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, table.colorbufferTable);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void drawCueStick(CueStick cueStick, GLuint MatrixID, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix){


	cueStick.MVP = ProjectionMatrix * ViewMatrix * cueStick.matrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &cueStick.MVP[0][0]);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, cueStick.vertexbufferStick);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, cueStick.colorbufferStick);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void cleanupBuffers(std::list<Ball*> listOfBalls){
	for (std::list<Ball*>::iterator currentBall = listOfBalls.begin(); currentBall != listOfBalls.end(); currentBall++){
		glDeleteBuffers(1, &(*currentBall)->vertexbuffer);
		glDeleteBuffers(1, &(*currentBall)->colorbuffer);
	}
}