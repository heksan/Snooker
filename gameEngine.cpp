// Include GLEW
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>   
#include <list>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

//include my files
#include "gameEngine.h"
#include "ball.h" 

const float ballRadius = 2.6f;
const float pocketRadius = 7.5f;
const float tableWidth = 89.0f;
const float tableLength = 178.5f;
const float precisionSteps = 200.0f;
const float decelerationRate = 0.02f;

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

void relocateMatrices(std::list<Pocket*> listOfPockets){

	for (std::list<Pocket*>::iterator currentPocket = listOfPockets.begin(); currentPocket != listOfPockets.end(); currentPocket++){
		(*currentPocket)->matrix = glm::translate((*currentPocket)->matrix, (*currentPocket)->position);
	}

}



//Checks if collisions happen and uses different methods depending on case
void checkBallCollisions(std::list<Ball*> listOfBalls){

	//loop checking collisions in a way there are no double comparisons
	for (std::list<Ball*>::iterator ballOne = listOfBalls.begin(); ballOne != listOfBalls.end(); ballOne++){
		for (std::list<Ball*>::iterator ballTwo = listOfBalls.begin(); ballTwo != listOfBalls.end(); ballTwo++){
		//for (std::list<Ball*>::iterator ballTwo = std::find(listOfBalls.begin(), listOfBalls.end(), (*ballOne)); ballTwo != listOfBalls.end(); ballTwo++){
			if ((*ballTwo)->id != (*ballOne)->id){//can and will be fixed
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
						std::cout << (*ballOne)->id << " and " << (*ballTwo)->id << " stuck, moving from " << glm::distance((*ballTwo)->ballPosition, (*ballOne)->ballPosition) << " to closest acceptable pos\n";
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
std::list<Ball*> checkWallCollisions(std::list<Ball*> listOfBalls,bool& foulCommited,int& currentPlayerID){

	//loop checking collisions in a way there are no double comparisons
	for (std::list<Ball*>::iterator ball = listOfBalls.begin(); ball != listOfBalls.end(); ball++){
		if ((abs((*ball)->ballPosition.x) >= tableWidth - ballRadius)){
			//determine wheter there is a pocket instead of wall
			if (abs((*ball)->ballPosition.z) >= pocketRadius && abs((*ball)->ballPosition.z) <= (tableLength - pocketRadius)){//change 7.5 as its a test value
				glm::vec3 stepBack = (*ball)->movementVector / precisionSteps * 2.0f *ballRadius;
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
			else{
				if ((*ball)->id == 0){
					foulCommited = true;
					std::cout << "foul, white in pocket \n";
					currentPlayerID = changePlayers(currentPlayerID);
				}
				listOfBalls = removeBall(listOfBalls, *ball);
				return listOfBalls;
			}
		}

		if (abs((*ball)->ballPosition.z) >= (tableLength - ballRadius)){
			if (abs((*ball)->ballPosition.x) <= (tableWidth - pocketRadius)){
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
			else{//ball in pocket
				if ((*ball)->id==0){
					foulCommited = true;
					std::cout << "foul, white in pocket \n";
					currentPlayerID = changePlayers(currentPlayerID);
				}
				listOfBalls = removeBall(listOfBalls, *ball);
				return listOfBalls;
			}
		}
	}
	return listOfBalls;
}


//If two balls are stuck together they are moved apart until distance between them is larger than 2* ballRadius
void collideStationary(Ball* ballOne, Ball* ballTwo){

	glm::vec3 movementVector = glm::normalize(glm::vec3(ballTwo->ballPosition.x - ballOne->ballPosition.x, 0.0f, ballTwo->ballPosition.z - ballOne->ballPosition.z));
	glm::vec3 stepBackOne = (movementVector / precisionSteps)*ballRadius*2.0f;
	glm::vec3 stepBackTwo = (movementVector / precisionSteps)*-ballRadius*2.0f;
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
			std::cout << " successfully moved back two stationary balls, new distance is" << glm::distance(ballOne->ballPosition, ballTwo->ballPosition) << "\n";
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
	for (int i = 0; i < precisionSteps*2; i++){// crossing on the further half of the ball requires more backtracking, hence *2
		newBallPosition = newBallPosition - stepBack;
		if (glm::distance(newBallPosition, ballTwo->ballPosition) > 2 * ballRadius){
			ballOne->matrix = glm::translate(ballOne->matrix, (newBallPosition - ballOne->ballPosition));
			ballOne->ballPosition = newBallPosition;
			//std::cout << " successfully moved back two balls, new distance is" << glm::distance(newBallPosition, ballTwo->ballPosition)<<"\n";
			break;
		}
	}
	//std::cout << " final distance is" << glm::distance(ballOne->ballPosition, ballTwo->ballPosition) << "\n";
	//calculate and set new movement vectors
	double angle = atan2(ballOne->ballPosition.x - ballTwo->ballPosition.x, ballOne->ballPosition.z - ballTwo->ballPosition.z);
	float speed = glm::length(ballOne->movementVector);
	glm::vec3 newMovementVector = glm::vec3(sin(angle + M_PI), 0, cos(angle + M_PI));
	ballTwo->movementVector = glm::normalize(newMovementVector)*speed*(0.5f);
	glm::vec3 vecThree = glm::normalize(ballOne->movementVector) - glm::normalize(ballTwo->movementVector);
	vecThree = glm::normalize(vecThree);
	ballOne->movementVector = vecThree*speed*(0.5f);

	recalculateDeceleration(ballOne);
	recalculateDeceleration(ballTwo);
}

//Balls switch movement vectors
void collideMoving(Ball *ballOne, Ball *ballTwo){

	glm::vec3 stepBackOne = (glm::normalize(ballOne->movementVector) *2.0f * ballRadius)/precisionSteps;
	glm::vec3 stepBackTwo = (glm::normalize(ballTwo->movementVector) *2.0f * ballRadius) / precisionSteps;
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
			//std::cout << " successfully moved back two moving balls, new distance is" << glm::distance(ballOne->ballPosition, ballTwo->ballPosition) << "\n";
			break;
		}
	}
	//std::cout << "two moving balls, new distance is" << glm::distance(ballOne->ballPosition, ballTwo->ballPosition) << "\n";
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
	float angleBallMouse = atan2(cueball.ballPosition.x - mouseRay.x, cueball.ballPosition.z - mouseRay.y);
	//trace back to zero
	moveStickToOrigin(cueStick);
	//translate to ball,rotate with mouse angle, translate by some value so stick points at the ball
	cueStick.matrix = glm::translate(cueStick.matrix, glm::vec3(cueball.ballPosition.x, 0.0f, cueball.ballPosition.z));
	cueStick.matrix = glm::rotate(cueStick.matrix, angleBallMouse, glm::vec3(0.0f, 1.0f, 0.0f));
	cueStick.matrix = glm::translate(cueStick.matrix, glm::vec3(0.0f, 0.0f, -cueStick.displacement));

	cueStick.position = glm::vec3(cueball.ballPosition.x, 0.0f, cueball.ballPosition.z);
	cueStick.temporaryPosition = cueStick.position;
	cueStick.rotation = angleBallMouse;


}

//after moving cueBall to staring position cue stick has to follow
void moveStickToOrigin(CueStick& cueStick){

	cueStick.matrix = glm::translate(cueStick.matrix, glm::vec3(0.0f, 0.0f, cueStick.displacement));
	cueStick.matrix = glm::rotate(cueStick.matrix, -cueStick.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	cueStick.matrix = glm::translate(cueStick.matrix, -cueStick.position);//goes to 0,0
}

//moves backwards slower than forward faster until hits the ball
void moveCueStick(CueStick& cueStick, float force, bool& ballsMoving, bool& cueStickMoving){

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
		if ((cueStick.position.z - cueStick.temporaryPosition.z) <= -7.0f){
			cueStick.matrix = glm::translate(cueStick.matrix, glm::vec3(0.0f, 0.0f, (cueStick.position.z - cueStick.temporaryPosition.z)));
			cueStickMoving = false;
			ballsMoving = true;
			cueStick.accelerating = false;
		}
	}
}

//removed ball from the list(by id)
std::list <Ball*> removeBall(std::list <Ball*> listOfBalls, Ball* ball){
	std::list<Ball*> newListOfBalls;
	for (std::list<Ball*>::iterator currentBall = listOfBalls.begin(); currentBall != listOfBalls.end(); currentBall++){
		if ((*currentBall)->id != ball->id){
			newListOfBalls.push_back(*currentBall);
		}
	}
	return newListOfBalls;
}

//Takes list of Balls which should be on the table after pocketing(in most cases), checks listOfBalls for their presence
//and if not found pushes them in
void replacePocketedBalls(std::list<Ball*>& listOfBalls, std::list <Ball*> listOfRepleacableBalls){
	for (std::list<Ball*>::iterator ballToBeInserted = listOfRepleacableBalls.begin(); ballToBeInserted != listOfRepleacableBalls.end(); ballToBeInserted++){
		bool found = false;
		for (std::list<Ball*>::iterator currentBall = listOfBalls.begin(); currentBall != listOfBalls.end(); currentBall++){
			if ((*currentBall)->id == (*ballToBeInserted)->id){
				found = true;
			}
		}
		if (!found){
			listOfBalls.push_front(*ballToBeInserted);
			moveBallToStartingPosition(*ballToBeInserted);
		}
	}
}

//after reinsertion checks inserted ball id and places it on designated position
/*Ball yellowBall(16, glm::vec3(30, 0, -100));
Ball orangeBall(17, glm::vec3(0, 0, -100));
Ball greenBall(18, glm::vec3(-30, 0, -100));
Ball blueBall(19, glm::vec3(0, 0, 0));
Ball pinkBall(20, glm::vec3(0, 0, 90));
Ball blackBall(21, glm::vec3(0, 0, 150));*/
void moveBallToStartingPosition(Ball *ballToBeInserted){
	if (ballToBeInserted->id == 0){
		ballToBeInserted->matrix = glm::translate(ballToBeInserted->matrix, -ballToBeInserted->ballPosition + glm::vec3(0.0f, 0.0f, -130.0f));
		ballToBeInserted->ballPosition = glm::vec3(0.0f, 0.0f, -130.0f);
		ballToBeInserted->movementVector = glm::vec3(0.0);
		
	}
	if (ballToBeInserted->id == 16){
		ballToBeInserted->matrix = glm::translate(ballToBeInserted->matrix, -ballToBeInserted->ballPosition + glm::vec3(30.0f, 0.0f, -100.0f));
		ballToBeInserted->ballPosition = glm::vec3(30.0f, 0.0f, -100.0f);
		ballToBeInserted->movementVector = glm::vec3(0.0);

	}
	if (ballToBeInserted->id == 17){
		ballToBeInserted->matrix = glm::translate(ballToBeInserted->matrix, -ballToBeInserted->ballPosition + glm::vec3(0.0f, 0.0f, -100.0f));
		ballToBeInserted->ballPosition = glm::vec3(0.0f, 0.0f, -100.0f);
		ballToBeInserted->movementVector = glm::vec3(0.0);

	}
	if (ballToBeInserted->id == 18){
		ballToBeInserted->matrix = glm::translate(ballToBeInserted->matrix, -ballToBeInserted->ballPosition + glm::vec3(-30.0f, 0.0f, -100.0f));
		ballToBeInserted->ballPosition = glm::vec3(-30.0f, 0.0f, -100.0f);
		ballToBeInserted->movementVector = glm::vec3(0.0);

	}
	if (ballToBeInserted->id == 19){
		ballToBeInserted->matrix = glm::translate(ballToBeInserted->matrix, -ballToBeInserted->ballPosition + glm::vec3(0.0f, 0.0f, 0.0f));
		ballToBeInserted->ballPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		ballToBeInserted->movementVector = glm::vec3(0.0);

	}
	if (ballToBeInserted->id == 20){
		ballToBeInserted->matrix = glm::translate(ballToBeInserted->matrix, -ballToBeInserted->ballPosition + glm::vec3(0.0f, 0.0f, 90.0f));
		ballToBeInserted->ballPosition = glm::vec3(0.0f, 0.0f, 90.0f);
		ballToBeInserted->movementVector = glm::vec3(0.0);

	}
	if (ballToBeInserted->id == 21){
		ballToBeInserted->matrix = glm::translate(ballToBeInserted->matrix, -ballToBeInserted->ballPosition + glm::vec3(0.0f, 0.0f, 150.0f));
		ballToBeInserted->ballPosition = glm::vec3(0.0f, 0.0f, 150.0f);
		ballToBeInserted->movementVector = glm::vec3(0.0);

	}
}


void relocateCueBall(glm::vec2 mouseRay, Ball& cueball){

	if (mouseRay.y < -100.0f && mouseRay.y > -178.0f && mouseRay.x > -89.0f && mouseRay.x < 89.0f){

		cueball.matrix = glm::translate(cueball.matrix, -cueball.ballPosition);

		cueball.ballPosition.x = mouseRay.x;
		cueball.ballPosition.z = mouseRay.y;

		cueball.matrix = glm::translate(cueball.matrix, cueball.ballPosition);
	}
}

int changePlayers(int currentPlayerID){
	if (currentPlayerID == 1){
		return 2;
	}
	else{
		return 1;
	}
}