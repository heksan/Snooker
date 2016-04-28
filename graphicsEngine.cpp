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
	glBufferData(GL_ARRAY_BUFFER, sizeof(table.vertexBufferDataTable), table.vertexBufferDataTable, GL_STATIC_DRAW);

	glGenBuffers(1, &table.uvbufferTable);
	glBindBuffer(GL_ARRAY_BUFFER, table.uvbufferTable);
	glBufferData(GL_ARRAY_BUFFER, sizeof(table.uvBufferDataTable), table.uvBufferDataTable, GL_STATIC_DRAW);

	glGenBuffers(1, &table.vertexbufferFrame);
	glBindBuffer(GL_ARRAY_BUFFER, table.vertexbufferFrame);
	glBufferData(GL_ARRAY_BUFFER, table.vertexBufferDataFrame.size()*sizeof(glm::vec3), &table.vertexBufferDataFrame[0], GL_STATIC_DRAW);

	glGenBuffers(1, &table.colorbufferFrame);
	glBindBuffer(GL_ARRAY_BUFFER, table.colorbufferFrame);
	glBufferData(GL_ARRAY_BUFFER, table.colorBufferDataFrame.size()*sizeof(glm::vec3), &table.colorBufferDataFrame[0], GL_STATIC_DRAW);

}

void createBuffer(CueStick& cueStick){

	glGenBuffers(1, &cueStick.vertexbufferStick);
	glBindBuffer(GL_ARRAY_BUFFER, cueStick.vertexbufferStick);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cueStick.vertex_buffer_data_stick)
		, cueStick.vertex_buffer_data_stick, GL_STATIC_DRAW);

	glGenBuffers(1, &cueStick.colorbufferStick);
	glBindBuffer(GL_ARRAY_BUFFER, cueStick.colorbufferStick);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cueStick.color_buffer_data_stick)
		, cueStick.color_buffer_data_stick, GL_STATIC_DRAW);
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

void createBuffers(std::list<Pocket*> listOfPockets){
	for (std::list<Pocket*>::iterator currentPocket = listOfPockets.begin(); currentPocket != listOfPockets.end(); currentPocket++){


		glGenBuffers(1, &(*currentPocket)->vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, (*currentPocket)->vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, (*currentPocket)->vertexBufferData.size()*sizeof(glm::vec3), &(*currentPocket)->vertexBufferData[0], GL_STATIC_DRAW);

		glGenBuffers(1, &(*currentPocket)->colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, (*currentPocket)->colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, (*currentPocket)->colorBufferData.size()*sizeof(glm::vec3), &(*currentPocket)->colorBufferData[0], GL_STATIC_DRAW);

	}
}



void drawBall(Ball ball, GLuint MatrixID, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix){


	ball.MVP = ProjectionMatrix * ViewMatrix * ball.matrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &ball.MVP[0][0]);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, ball.vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. 0 for vert 1 for col
		3,                  // size of data(3 vars per point)
		GL_FLOAT,           // type
		GL_FALSE,           // no idea
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, ball.colorbuffer);
	glVertexAttribPointer(
		1,                               
		3,                                
		GL_FLOAT,                        
		GL_FALSE,                         
		0,                               
		(void*)0                         
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

void drawPockets(std::list<Pocket*> listOfPockets, GLuint MatrixID, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix){
	for (std::list<Pocket*>::iterator currentPocket = listOfPockets.begin(); currentPocket != listOfPockets.end(); currentPocket++){
		(*currentPocket)->MVP = ProjectionMatrix * ViewMatrix * (*currentPocket)->matrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(*currentPocket)->MVP[0][0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, (*currentPocket)->vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, (*currentPocket)->colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, (*currentPocket)->vertexBufferData.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}
//only draw here with texture
void drawTable(Table table, GLuint MatrixID, GLuint TextureID, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, GLuint tableTexture){

	table.MVP = ProjectionMatrix * ViewMatrix * table.matrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &table.MVP[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D,tableTexture);
	glUniform1i(TextureID, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, table.vertexbufferTable);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, table.uvbufferTable);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);


}

void drawFrame(Table table, GLuint MatrixID, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix){

	table.MVP = ProjectionMatrix * ViewMatrix * table.matrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &table.MVP[0][0]);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, table.vertexbufferFrame);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, table.colorbufferFrame);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, table.vertexBufferDataFrame.size());

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

void cleanupBuffers(std::list<Pocket*> listOfPockets){
	for (std::list<Pocket*>::iterator currentPocket = listOfPockets.begin(); currentPocket != listOfPockets.end(); currentPocket++){
		glDeleteBuffers(1, &(*currentPocket)->vertexbuffer);
		glDeleteBuffers(1, &(*currentPocket)->colorbuffer);
	}
}