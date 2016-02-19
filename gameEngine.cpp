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
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include "gameEngine.h"
#include "ball.h" 

const float ballRadius = 2.6f;
const float tableWidth = 89.0f;
const float tableLength = 178.5f;
const float precisionSteps = 200.0f;
const float decelerationRate = 0.08f;

glm::mat4 moveBall(Ball& ball){

	ball.matrix = glm::translate(ball.matrix, ball.movementVector);
	ball.ballPosition = ball.ballPosition + ball.movementVector;
	ball.movementVector.x = ball.movementVector.x*0.98f;
	ball.movementVector.z = ball.movementVector.z*0.98f;
	return ball.matrix;
}

//moved all balls - changes matrices, positions and decelerates movementVector
void moveBalls(std::list<Ball*> listOfBalls){
	for (std::list<Ball*>::iterator currentBall = listOfBalls.begin(); currentBall != listOfBalls.end(); currentBall++){

		if (glm::length((*currentBall)->movementVector) != 0){
			(*currentBall)->matrix = glm::translate((*currentBall)->matrix, (*currentBall)->movementVector);
			(*currentBall)->ballPosition = (*currentBall)->ballPosition + (*currentBall)->movementVector;
			(*currentBall)->movementVector.x = (*currentBall)->movementVector.x - (*currentBall)->deceleration.x;
			(*currentBall)->movementVector.z = (*currentBall)->movementVector.z - (*currentBall)->deceleration.z;
		}
	}
}

//Used to set initial movementVector and deceleration to cueBall,
//movementVector is currently based on mouse position 
void initMovement(float speedVec, glm::vec2 mouseRay, Ball& ball){

	double angle = atan2(mouseRay.x - ball.ballPosition.x, mouseRay.y - ball.ballPosition.z);
	ball.movementVector = glm::vec3(sin(angle + M_PI), 0, cos(angle + M_PI));
	ball.movementVector = ball.movementVector * speedVec;
	ball.deceleration = glm::normalize(ball.movementVector)*decelerationRate;
}


bool checkStable(Ball ball){
	return glm::length(ball.movementVector) <= 1.0;
}

//Iterates through all balls, if any movementVector is > than 0 it returns false
bool checkStable(std::list<Ball*> listOfBalls){

	for (std::list<Ball*>::iterator currentBall = listOfBalls.begin(); currentBall != listOfBalls.end(); currentBall++){
		if (glm::length((*currentBall)->movementVector) != 0){
			return true;
		}
	}
	std::cout << "stable \n";
	return false;
}

//Iterates through all balls, if angle between balls movementVector and deceleration is larger than 0,
//hence in opposite direction, it sets both values to 0,0,0(stops the ball)
void checkStop(std::list<Ball*> listOfBalls){

	for (std::list<Ball*>::iterator currentBall = listOfBalls.begin(); currentBall != listOfBalls.end(); currentBall++){
		if (glm::angle(glm::normalize((*currentBall)->movementVector), glm::normalize((*currentBall)->deceleration)) > 0.01f){
			(*currentBall)->movementVector = glm::vec3(0.0f, 0.0f, 0.0f);
			(*currentBall)->deceleration = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}
}

void relocateMatrix(Ball& ball){
	ball.matrix = glm::translate(ball.matrix, ball.ballPosition);
}

//Initial matrix of any ball is centered at 0,0,0, , function moves it to ball.position
void relocateMatrices(std::list<Ball*> listOfBalls){

	for (std::list<Ball*>::iterator currentBall = listOfBalls.begin(); currentBall != listOfBalls.end(); currentBall++){
		(*currentBall)->matrix = glm::translate((*currentBall)->matrix, (*currentBall)->ballPosition);
	}

}



//Checks if collisions happen and uses different methods depending on case
void checkBallCollisions(std::list<Ball*> listOfBalls){

	//loop checking collisions in a way there are no double comparisons
	for (std::list<Ball*>::iterator ballOne = listOfBalls.begin(); ballOne != listOfBalls.end(); ballOne++){
		for (std::list<Ball*>::iterator ballTwo = std::find(listOfBalls.begin(), listOfBalls.end(), (*ballOne)); ballTwo != listOfBalls.end(); ballTwo++){
			if ((*ballTwo)->id != (*ballOne)->id){
				if (glm::distance((*ballTwo)->ballPosition, (*ballOne)->ballPosition) < 2 * ballRadius){
					//4 ways of ensuring no infinite collisions - happens when ball travel too fast and is caught "inside" other ball forever
					//slowing down(solves part of problem)
					//bounding box larger than diameter(bad idea)
					//noclip with the same target for x frames(bad idea)
					//------>moving balls to exact position(where they should be) on collision  - not most accurate but its not rocket science, it can be off for 0.002f
					//could also precompute bounces and execute based on frame/time but seems too troublesome(most accurate tho and no infinite collisions) - will do if enough time

					//both stationary,caused by backtracking of other balls
					if (glm::length((*ballOne)->movementVector) == 0.0 && (glm::length((*ballTwo)->movementVector) == 0.0)){
						collideStationary(*ballOne, *ballTwo);
					}
					//both moving
					if (glm::length((*ballOne)->movementVector) > 0.0 && (glm::length((*ballTwo)->movementVector) > 0.0)){
						collideMoving(*ballOne, *ballTwo);
					}
					//one moving
					if (glm::length((*ballOne)->movementVector) > 0.0 && (glm::length((*ballTwo)->movementVector) == 0.0)){
						collideOneMoving(*ballOne, *ballTwo);
					}
					if (glm::length((*ballTwo)->movementVector) > 0.0 && (glm::length((*ballTwo)->movementVector) == 0.0)){
						collideOneMoving(*ballTwo, *ballOne);
					}
				}
			}
		}
	}
}

//When ball moves slightly outside of table area it is moved back to closes "in-table" position and has its movement vector changed
void checkWallCollisions(std::list<Ball*> listOfBalls){

	//loop checking collisions in a way there are no double comparisons
	for (std::list<Ball*>::iterator ball = listOfBalls.begin(); ball != listOfBalls.end(); ball++){
		if (abs((*ball)->ballPosition.x) >= tableWidth - ballRadius){
			//trace back to contact point
			glm::vec3 stepBack = (*ball)->movementVector / precisionSteps;
			glm::vec3 newBallPosition = (*ball)->ballPosition;

			for (int i = 0; i < precisionSteps; i++){
				newBallPosition = newBallPosition - (stepBack*2.0f);
				if (abs(newBallPosition.x) <= tableWidth - ballRadius){
					(*ball)->matrix = glm::translate((*ball)->matrix, (newBallPosition - (*ball)->ballPosition));
					(*ball)->ballPosition = newBallPosition;
					break;
				}
			}
			changeDirection(*ball, 'x');
		}

		if (abs((*ball)->ballPosition.z) >= (tableLength - ballRadius)){
			//trace back to contact point
			glm::vec3 stepBack = (*ball)->movementVector / precisionSteps;
			glm::vec3 newBallPosition = (*ball)->ballPosition;
			for (int i = 0; i < precisionSteps; i++){
				newBallPosition = newBallPosition - (stepBack*2.0f);
				if (abs(newBallPosition.z) <= tableLength - ballRadius){
					(*ball)->matrix = glm::translate((*ball)->matrix, (newBallPosition - (*ball)->ballPosition));
					(*ball)->ballPosition = newBallPosition;
					break;
				}

			}
			changeDirection(*ball, 'z');
		}
	}
}


//If two balls are stuck together they are moved apart until distance between them is larger than 2* ballRadius
void collideStationary(Ball* ballOne, Ball* ballTwo){

	glm::vec3 movementVector = glm::normalize(glm::vec3(ballTwo->ballPosition.x - ballOne->ballPosition.x, 0.0f, ballTwo->ballPosition.z - ballOne->ballPosition.z));
	glm::vec3 stepBackOne = (movementVector / precisionSteps)*ballRadius;
	glm::vec3 stepBackTwo = (movementVector / precisionSteps)*-ballRadius;
	glm::vec3 newBallOnePosition = ballOne->ballPosition;
	glm::vec3 newBallTwoPosition = ballTwo->ballPosition;

	for (int i = 0; i < precisionSteps; i++){
		newBallOnePosition = newBallOnePosition - stepBackOne;
		newBallTwoPosition = newBallTwoPosition - stepBackTwo;
		if ((glm::distance(newBallOnePosition, newBallTwoPosition) > 2.0f * ballRadius)){
			ballOne->matrix = glm::translate(ballOne->matrix, (newBallOnePosition - ballOne->ballPosition));
			ballTwo->matrix = glm::translate(ballTwo->matrix, (newBallTwoPosition - ballTwo->ballPosition));
			ballOne->ballPosition = newBallOnePosition;
			ballTwo->ballPosition = newBallTwoPosition;
			break;
		}
	}
}



//Var order - ball hitting, ball hitted
//only ball hitting is moved backwards until distance is > 2*ballRadius - more precise than moving 2 backwards
//assumes no rotation hence 90deg between balls after collision, kinetic energy is equally divided between balls - same speed
void collideOneMoving(Ball* ballOne, Ball* ballTwo){

	glm::vec3 stepBack = (glm::normalize(ballOne->movementVector) / precisionSteps)*ballRadius*2.0f;
	glm::vec3 newBallPosition = ballOne->ballPosition;
	//trace back to collision point
	for (int i = 0; i < precisionSteps; i++){
		newBallPosition = newBallPosition - stepBack;
		if (glm::distance(newBallPosition, ballTwo->ballPosition) > 2 * ballRadius){
			ballOne->matrix = glm::translate(ballOne->matrix, (newBallPosition - ballOne->ballPosition));
			ballOne->ballPosition = newBallPosition;
			break;
		}
	}

	//calculate and set new movement vectors
	double angle = atan2(ballOne->ballPosition.x - ballTwo->ballPosition.x, ballOne->ballPosition.z - ballTwo->ballPosition.z);
	float speed = glm::length(ballOne->movementVector);
	glm::vec3 newMovementVector = glm::vec3(sin(angle + M_PI), 0, cos(angle + M_PI));
	ballTwo->movementVector = glm::normalize(newMovementVector)*speed*(0.5f);
	glm::vec3 vecThree = ballOne->movementVector - ballTwo->movementVector;
	vecThree = glm::normalize(vecThree);
	ballOne->movementVector = vecThree*speed*(0.5f);

	recalculateDeceleration(ballOne);
	recalculateDeceleration(ballTwo);
}

//Balls switch movement vectors
void collideMoving(Ball *ballOne, Ball *ballTwo){

	std::cout << "movng collided \n ";
	glm::vec3 stepBackOne = (ballOne->movementVector / precisionSteps)*2.0f;
	glm::vec3 stepBackTwo = (ballTwo->movementVector / precisionSteps)*2.0f;
	glm::vec3 newBallOnePosition = ballOne->ballPosition;
	glm::vec3 newBallTwoPosition = ballTwo->ballPosition;
	//trace back to collision point
	for (int i = 0; i < precisionSteps; i++){
		newBallOnePosition = newBallOnePosition - stepBackOne;
		newBallTwoPosition = newBallTwoPosition - stepBackTwo;
		if ((glm::distance(newBallOnePosition, newBallTwoPosition) > 2 * ballRadius)){
			ballOne->matrix = glm::translate(ballOne->matrix, (newBallOnePosition - ballOne->ballPosition));
			ballTwo->matrix = glm::translate(ballTwo->matrix, (newBallTwoPosition - ballTwo->ballPosition));
			ballOne->ballPosition = newBallOnePosition;
			ballTwo->ballPosition = newBallTwoPosition;
			recalculateDeceleration(ballOne);
			recalculateDeceleration(ballTwo);
			break;
		}
	}

	glm::vec3 placeholder = ballOne->movementVector;
	ballOne->movementVector = ballTwo->movementVector;
	ballTwo->movementVector = placeholder;
}

//self explanatory
void changeDirection(Ball* ball, char xORz){

	if (xORz == 'x'){
		ball->movementVector.x = -ball->movementVector.x;
		recalculateDeceleration(ball);

	}
	if (xORz == 'z'){
		ball->movementVector.z = -ball->movementVector.z;
		recalculateDeceleration(ball);
	}
}

//self explanatory
void recalculateDeceleration(Ball* ball){
	(ball)->deceleration = glm::normalize((ball)->movementVector)*decelerationRate;
}

void relocateCueStick(glm::vec2 mouseRay, CueStick& cueStick, Ball cueball){
	float angleBallMouse = atan2(cueball.ballPosition.x-mouseRay.x,cueball.ballPosition.z - mouseRay.y);
	//trace back to zero
	moveStickToOrigin(cueStick);
	//translate to ball,rotate with mouse angle, translate by some value so stick points at the ball
	cueStick.matrix = glm::translate(cueStick.matrix, glm::vec3(cueball.ballPosition.x, 0.0f, cueball.ballPosition.z));
	cueStick.matrix = glm::rotate(cueStick.matrix, angleBallMouse, glm::vec3(0.0f, 1.0f, 0.0f));
	cueStick.matrix = glm::translate(cueStick.matrix, glm::vec3(0.0f, 0.0f, -cueStick.displacement ));
	
	cueStick.position = glm::vec3(cueball.ballPosition.x, 0.0f, cueball.ballPosition.z);
	cueStick.temporaryPosition = cueStick.position;
	cueStick.rotation = angleBallMouse;
	

}

void moveStickToOrigin(CueStick& cueStick){
	cueStick.matrix = glm::translate(cueStick.matrix, glm::vec3(0.0f, 0.0f, cueStick.displacement));
	cueStick.matrix = glm::rotate(cueStick.matrix, -cueStick.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	cueStick.matrix = glm::translate(cueStick.matrix, -cueStick.position);//goes to 0,0
}


void moveCueStick(CueStick& cueStick, float force,bool& ballsMoving, bool& cueStickMoving){

	if (!cueStick.accelerating){
		cueStick.matrix = glm::translate(cueStick.matrix, glm::vec3(0.0f, 0.0f, -0.1f*force));
		cueStick.temporaryPosition = cueStick.temporaryPosition + glm::vec3(0.0f, 0.0f, -0.1f*force);
		if (glm::distance(cueStick.position, cueStick.temporaryPosition) >= force*2.0f){
			cueStick.accelerating = true;
		}
	}
	else{
		cueStick.matrix = glm::translate(cueStick.matrix, glm::vec3(0.0f, 0.0f, 0.3f*force));
		cueStick.temporaryPosition = cueStick.temporaryPosition + glm::vec3(0.0f, 0.0f, 0.3f*force);
		std::cout << (cueStick.position.z - cueStick.temporaryPosition.z) << "\n";
		if ((cueStick.position.z - cueStick.temporaryPosition.z) <= -7.0f){
			cueStick.matrix = glm::translate(cueStick.matrix, glm::vec3(0.0f, 0.0f, (cueStick.position.z - cueStick.temporaryPosition.z)));
			cueStickMoving = false;
			ballsMoving = true;
			cueStick.accelerating = false;
		}
	}
	
	
	//cueStick.position = cueStick.position + glm::vec3(0.0f, 0.0f, 0.1f*force);

}
