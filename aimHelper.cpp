// Include GLEW
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h> 

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "aimHelper.h"
#include "gameEngine.h"
#include "ball.h"
#include <list>
#include <vector>

float const helperSize = 0.2f;

bool secondBallMoving;
int  collisionNum = 0;
//a template for hit ball
Ball cpyOfBall(1, glm::vec3(0, 0, 0));


GLuint aimHelperVertexBufferID;
GLuint aimHelperColorBufferID;
GLuint shader;
GLuint uniformAimHelper;
glm::mat4 MVP;
glm::mat4 matrix;
const float ballRadius = 2.6f;
const float pocketRadius = 3.5f;
const float tableWidth = 89.0f;
const float tableLength = 178.5f;

void initAimHelper(GLuint shader){
	glGenBuffers(1, &aimHelperVertexBufferID);
	glGenBuffers(1, &aimHelperColorBufferID);
	uniformAimHelper = glGetUniformLocation(shader, 0);
}

void drawAimHelper(std::vector<glm::vec3> helper, glm::vec3 position, GLuint MatrixID ,glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix){
	std::vector<glm::vec3> drawData;
	std::vector<glm::vec3> drawColor;
	for (std::vector<glm::vec3>::iterator help = helper.begin(); help != helper.end(); help++){
		drawData.push_back(*help + glm::vec3(helperSize, 0, helperSize));
		drawColor.push_back(glm::vec3(0, 0, 0));
		drawData.push_back(*help + glm::vec3(helperSize, 0, -helperSize));
		drawColor.push_back(glm::vec3(0, 0, 0));
		drawData.push_back(*help + glm::vec3(-helperSize, 0, -helperSize));
		drawColor.push_back(glm::vec3(0, 0, 0));
		drawData.push_back(*help + glm::vec3(-helperSize, 0, -helperSize));
		drawColor.push_back(glm::vec3(0, 0, 0));
		drawData.push_back(*help + glm::vec3(helperSize, 0, helperSize));
		drawColor.push_back(glm::vec3(0, 0, 0));
		drawData.push_back(*help + glm::vec3(-helperSize, 0, helperSize));
		drawColor.push_back(glm::vec3(0, 0, 0));
	}
	glBindBuffer(GL_ARRAY_BUFFER, aimHelperVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, drawData.size()* sizeof(glm::vec3), &drawData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, aimHelperColorBufferID);
	glBufferData(GL_ARRAY_BUFFER, drawColor.size()* sizeof(glm::vec3), &drawColor[0], GL_STATIC_DRAW);

	MVP = ProjectionMatrix * ViewMatrix * matrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, aimHelperVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, aimHelperColorBufferID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, drawData.size());
	drawData.clear();
	drawColor.clear();
}



std::vector<glm::vec3> calcAimHelper(glm::vec2 mouseRay, Ball cueBall, std::list<Ball*> listOfBalls){ //std::vector<glm::vec3>
	//add wall collisions, move draw to graphics engine

	std::vector<glm::vec3> helperPoints;
	collisionNum = 0;
	secondBallMoving = false;

	//calculate initial movement vector and make duplacate of qBall
	double angle = atan2(mouseRay.x - cueBall.ballPosition.x, mouseRay.y - cueBall.ballPosition.z);
	glm::vec3 cueMovement = glm::vec3(sin(angle + M_PI), 0, cos(angle + M_PI));
	cueMovement = cueMovement * 0.5f;
	glm::vec3 cuePosition = cueBall.ballPosition;
	Ball cue(0, cuePosition);
	cue.ballPosition = cue.ballPosition + cue.movementVector;
	cue.movementVector = cueMovement;

	for (int i = 0; i < 400; i++){
		cue.ballPosition = cue.ballPosition + cue.movementVector;
		secondBallMoving = checkCueCollision(cue, listOfBalls);
		shoterWallCollision(cue);
		if (i % 10 == 0){
			helperPoints.push_back(cue.ballPosition);
		}
	}
	if (secondBallMoving){
		for (int i = 0; i < 200; i++){
			cpyOfBall.ballPosition = cpyOfBall.ballPosition + cpyOfBall.movementVector;
			shoterWallCollision(cpyOfBall);
			if (i % 10 == 0){
				helperPoints.push_back(cpyOfBall.ballPosition);
			}
		}
	}
	return helperPoints;

}

bool checkCueCollision(Ball& cue, std::list<Ball*> listOfBalls){
	for (std::list<Ball*>::iterator ball = listOfBalls.begin(); ball != listOfBalls.end(); ball++){
		if ((*ball)->id != 0 && (glm::distance(cue.ballPosition, (*ball)->ballPosition) < 2 * 2.6f)){
			collisionNum += 1;
			if (collisionNum <= 1){
				cpyOfBall.ballPosition = (*ball)->ballPosition;
				cpyOfBall.movementVector = (*ball)->movementVector;
				collideOneMoving(&cue, &cpyOfBall);
				secondBallMoving = true;
			}
		}

	}
	return secondBallMoving;
		
}

void shoterWallCollision(Ball& ball){
	if ((abs(ball.ballPosition.x) >= (tableWidth - ballRadius))){
		if (abs(ball.ballPosition.z) >= pocketRadius && abs(ball.ballPosition.z) <= (tableLength - pocketRadius)){
			changeDirection(&ball, 'x');
		}
	}
	if (abs(ball.ballPosition.z) >= (tableLength - ballRadius)){
		if (abs(ball.ballPosition.x <= (tableWidth - pocketRadius))){
			changeDirection(&ball, 'z');
		}
	}
}