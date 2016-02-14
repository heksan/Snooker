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
using namespace glm;

#include "gameEngine.h"
#include "ball.h" 

float ballRadius = 2.6f;
float tableWidth = 89.0f;
float tableLength = 178.5f;

glm::mat4 moveBall(Ball& ball){
	//std::cout << "obj values\n";
	//std::cout << ball.movementVector.x;
	//std::cout << "\n";
	//std::cout << ball.movementVector.z;
	//std::cout << "\n";
	ball.matrix = glm::translate(ball.matrix, ball.movementVector);
	ball.ballPosition = ball.ballPosition + ball.movementVector;
	ball.movementVector.x = ball.movementVector.x*0.98; //later change deceleration to cont value - v=v0-time*dece
	ball.movementVector.z = ball.movementVector.z*0.98;

	return ball.matrix;

}
void moveBalls(std::list<Ball*> listOfBalls){
	for (std::list<Ball*>::iterator currentBall = listOfBalls.begin(); currentBall != listOfBalls.end(); currentBall++){
		
		//std::cout << "list values\n";
		//std::cout << (*currentBall)->movementVector.x;
		//std::cout << "\n";
		//std::cout << (*currentBall)->movementVector.z;
		//std::cout << "\n";
		(*currentBall)->matrix = glm::translate((*currentBall)->matrix, (*currentBall)->movementVector);
		(*currentBall)->ballPosition = (*currentBall)->ballPosition + (*currentBall)->movementVector;
		(*currentBall)->movementVector.x = (*currentBall)->movementVector.x *0.999;
		(*currentBall)->movementVector.z = (*currentBall)->movementVector.z *0.999;
	}
}

void initMovement(float speedVec,glm::vec3 cameraPosition,glm::vec3 ballPosition,glm::vec3& movementVector){
	double angle = atan2(cameraPosition.x - ballPosition.x, cameraPosition.z - ballPosition.z);	
	movementVector = glm::vec3(sin(angle + M_PI), 0, cos(angle + M_PI));
	movementVector = movementVector * speedVec;
	

}

//change for stack later
bool checkStable(Ball ball){// later stack
	if (glm::length(ball.movementVector) <= 1.0){
		std::cout << "stable true\n";
		return true;
	}
	else
	{
		return false; 
	}
}

bool checkStable(std::list<Ball*> listOfBalls){
	for (std::list<Ball*>::iterator currentBall = listOfBalls.begin(); currentBall != listOfBalls.end(); currentBall++){
		//std::cout << glm::length((*currentBall)->movementVector)<<"   "<<(*currentBall)->id <<"\n";
		if (glm::length((*currentBall)->movementVector) != 0){
			if (glm::length((*currentBall)->movementVector) < 0.1){
				(*currentBall)->movementVector = glm::vec3(0, 0, 0);
				//std::cout << (*currentBall)->id << " stopped \n";
			}
			return false;
		}

	}
	std::cout << "stable \n";
	return true;
}





void relocateMatrix(Ball& ball){
		ball.matrix = glm::translate(ball.matrix, ball.ballPosition);
}


void relocateMatrices(std::list<Ball*> listOfBalls){
	for (std::list<Ball*>::iterator currentBall = listOfBalls.begin(); currentBall != listOfBalls.end(); currentBall++){
		(*currentBall)->matrix = glm::translate((*currentBall)->matrix, (*currentBall)->ballPosition);
	}
	
}



//do casy study
void checkBallCollisions(std::list<Ball*> listOfBalls){ // can be optimized in many ways( run through every comparison and drop it when speed == 0) , comment this

	//loop checking collisions in a way there are no double comparisons
	for (std::list<Ball*>::iterator ballOne = listOfBalls.begin(); ballOne != listOfBalls.end(); ballOne++){
		for (std::list<Ball*>::iterator ballTwo = std::find(listOfBalls.begin(), listOfBalls.end(),(*ballOne)); ballTwo != listOfBalls.end(); ballTwo++){
			if ((*ballTwo)->id != (*ballOne)->id){
				
					if (glm::distance((*ballTwo)->ballPosition, (*ballOne)->ballPosition) < 2*ballRadius){
						//4 ways of ensuring no infinite collisions - happen when ball travel too fast and is caught "inside" other ball forever
						//slowing the fuck down(solves part of problem)
						// bounding box larger than diameter(most meh idea ever)
						//noclip with the same target for x frames(stupid but would work)
						//------>moving balls to exact position(where they should be) on collision w
						//(should be different for each collision so code in "collide" methods but no because collision detection itself is inaccurate af)

						//could also precompute bounces and execute based on frame/time but seems too troublesome(most accurate tho and no infinite collisions)
						//same shit goes for wall collisions

						//decide whether both balls were moving
						if (glm::length((*ballOne)->movementVector) > 0.0 && (glm::length((*ballTwo)->movementVector) > 0.0)){
							collideMoving(*ballOne, *ballTwo);
						}
						else{//only one moving, decide which ball is being hit(angles matter)
							if (glm::length((*ballOne)->movementVector) > 0.0){

								collideStationary(*ballOne, *ballTwo);
							}
							else{
								collideStationary(*ballTwo,*ballOne);
							}




						}
					}
		
			}
		}
	}
}
void checkWallCollisions(std::list<Ball*> listOfBalls){ // can be optimized in many ways( run through every comparison and drop it when speed == 0) , comment this

	//loop checking collisions in a way there are no double comparisons
	for (std::list<Ball*>::iterator ball = listOfBalls.begin(); ball != listOfBalls.end(); ball++){
		
		if (abs((*ball)->ballPosition.x) >= tableWidth - ballRadius){
			//trace back to contact point
			glm::vec3 stepBack = (*ball)->movementVector / 200.0f;
			glm::vec3 newBallPosition = (*ball)->ballPosition;
			for (int i = 0; i < 200; i++){
				newBallPosition = newBallPosition - stepBack;
				if (newBallPosition.x <= tableWidth - ballRadius){
					//std::cout << "moved back from wall " << i << "  times, new distance is(the closer to 5.2 the better) :"
					//	<< glm::distance(newBallPosition, (*ball)->ballPosition) << "\n";
					(*ball)->matrix = glm::translate((*ball)->matrix, (newBallPosition - (*ball)->ballPosition));
					(*ball)->ballPosition = newBallPosition;
					break;
				}
			}

			changeDirection(*ball, 'x');
		}

		if (abs((*ball)->ballPosition.z) >= (tableLength - ballRadius)){
			//trace back to contact point
			glm::vec3 stepBack = (*ball)->movementVector / 200.0f;
			glm::vec3 newBallPosition = (*ball)->ballPosition;
			for (int i = 0; i < 200; i++){
				newBallPosition = newBallPosition - stepBack;
				if (newBallPosition.z <= tableLength - ballRadius){
					//std::cout << "moved back from wall " << i << "  times, new distance is(the closer to 5.2 the better) :"
					//	<< glm::distance(newBallPosition, (*ball)->ballPosition) << "\n";
					(*ball)->matrix = glm::translate((*ball)->matrix, (newBallPosition - (*ball)->ballPosition));
					(*ball)->ballPosition = newBallPosition;
					break;
				}

			}
			changeDirection(*ball, 'z');
		}
	}
}


//ball hitting, ball hitted
void collideStationary(Ball* ballOne, Ball* ballTwo){//assumes no rotation hence 90deg between
	//finding posint of collision(not accurate, can be fixed later, i cant into maths today)
	glm::vec3 stepBack = ballOne->movementVector / 200.0f;
	glm::vec3 newBallPosition = ballOne->ballPosition;
	for (int i = 0; i < 200;i++){
		newBallPosition = newBallPosition - stepBack;
		if (glm::distance(newBallPosition, ballTwo->ballPosition) > 2 * ballRadius){
			//std::cout << "moved back  " << i << "  times, new distance is(the closer to 5.2 the better) :" << glm::distance(newBallPosition, ballTwo->ballPosition) << "\n";
			ballOne->matrix = glm::translate(ballOne->matrix, (newBallPosition - ballOne->ballPosition));
			ballOne->ballPosition = newBallPosition;
			break;
		}
	}

	//rest of code
	double angle = atan2(ballOne->ballPosition.x - ballTwo->ballPosition.x, ballOne->ballPosition.z - ballTwo->ballPosition.z);
	float speed = glm::length(ballOne->movementVector);
	glm::vec3 movementVector = glm::vec3(sin(angle + M_PI), 0, cos(angle + M_PI));
	ballTwo->movementVector = glm::normalize(movementVector)*speed*(0.5f);
	//more elegant solution
	glm::vec3 vecThree = ballOne->movementVector - ballTwo->movementVector;
	vecThree = glm::normalize(vecThree);
	ballOne->movementVector = vecThree*speed*(0.5f);
	//std::cout << "two stationary balls collided, after switching speeds were : " << glm::length(ballOne->movementVector) << " and "
		//<< glm::length(ballTwo->movementVector)<<"\n";



	//glm::vec3 vecOne = -glm::cross(ballTwo->movementVector, vec3(0.0, 1.0, 0.0)); 
	//glm::vec3 vecTwo = glm::cross(ballTwo->movementVector, vec3(0.0, 1.0, 0.0));
	//vecOne = glm::normalize(vecOne);
	//vecTwo = glm::normalize(vecTwo);

	/*if (abs(glm::orientedAngle(vecOne, glm::normalize(ballOne->movementVector), glm::vec3(0, 1, 0))) 
		< abs(glm::orientedAngle(vecTwo, glm::normalize(ballOne->movementVector), glm::vec3(0, 1, 0)))){//finds one closer to original trajectory
		//std::cout << glm::orientedAngle(vecOne, glm::normalize(ballOne->movementVector), glm::vec3(0, 1, 0)) << "right hit\n";
		//std::cout << glm::orientedAngle(vecTwo, glm::normalize(ballOne->movementVector), glm::vec3(0, 1, 0)) << "\n";
		ballOne->movementVector = vecOne*speed*(0.5f);
	}
	else{
		
		//std::cout << glm::orientedAngle(vecOne, glm::normalize(ballOne->movementVector), glm::vec3(0, 1, 0)) << "right hit\n";
		//std::cout << glm::orientedAngle(vecTwo, glm::normalize(ballOne->movementVector), glm::vec3(0, 1, 0)) << "\n";
		ballOne->movementVector = vecTwo*speed*(0.5f);
	}*/
	
}

void collideMoving(Ball *ballOne,Ball *ballTwo){
	glm::vec3 stepBackOne = ballOne->movementVector / 200.0f;
	glm::vec3 stepBackTwo = ballTwo->movementVector / 200.0f;
	glm::vec3 newBallOnePosition = ballOne->ballPosition;
	glm::vec3 newBallTwoPosition = ballTwo->ballPosition;
	for (int i = 0; i < 200; i++){
		newBallOnePosition = newBallOnePosition - stepBackOne;
		newBallTwoPosition = newBallTwoPosition - stepBackTwo;
		if ((glm::distance(newBallOnePosition,newBallTwoPosition) > 2 * ballRadius)){
			//std::cout << "moved back  " << i << "  times, new distance is(the closer to 5.2 the better) :" 
			//	<< glm::distance(newBallOnePosition, newBallTwoPosition) << "\n";
			ballOne->matrix = glm::translate(ballOne->matrix, (newBallOnePosition - ballOne->ballPosition));
			ballTwo->matrix = glm::translate(ballTwo->matrix, (newBallTwoPosition - ballTwo->ballPosition));
			ballOne->ballPosition = newBallOnePosition;
			ballTwo->ballPosition = newBallTwoPosition;
			break;
		}
	}



	glm::vec3 placeholder = ballOne->movementVector;
	ballOne->movementVector = ballTwo->movementVector;
	ballTwo->movementVector = placeholder;
	//std::cout << "two moving balls collided, after switching speeds were : " << glm::length(ballOne->movementVector)
		//<< " and " << glm::length(ballTwo->movementVector) << "\n";
}


void changeDirection(Ball* ball, char xORz){
	if (xORz =='x'){
		ball->movementVector.x = -ball->movementVector.x;
	}
	if (xORz == 'z'){
		ball->movementVector.z = -ball->movementVector.z;
	}
}