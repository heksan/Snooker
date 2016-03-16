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
#include "ball.h"
#include <list>
#include <vector>



GLuint aimHelperVertexBufferID;
GLuint aimHelperColorBufferID;
GLuint shader;
GLuint uniformAimHelper;

void initAimHelper(GLuint shader){
	glGenBuffers(1, &aimHelperVertexBufferID);
	glGenBuffers(1, &aimHelperColorBufferID);
	uniformAimHelper = glGetUniformLocation(shader, 0);

}

void drawAimHelper(){

}


void calcAimHelper(glm::vec2 mouseRay,Ball cueBall, std::list<Ball*> listOfBalls){
	//take vector from init movement and follow positions for 100 frames or until second collision

	std::vector<glm::vec3> helperPoints;


	/* double angle = atan2(mouseRay.x - ball.ballPosition.x, mouseRay.y - ball.ballPosition.z);
	ball.movementVector = glm::vec3(sin(angle + M_PI), 0, cos(angle + M_PI));
	ball.movementVector = ball.movementVector * speedVec;
	*/

	//moveball (s) if two , set movement vector to sth normal
	/*	ball.matrix = glm::translate(ball.matrix, ball.movementVector);
	ball.ballPosition = ball.ballPosition + ball.movementVector;
	return ball.matrix;*/

	//check collisions
	//if collided with ball calc first collision and rtace new ball for 50 frames or until nex collision

	//save pos of both(or one) in one vec (size 100 or 150)
	//helperPoints.push_back(something);




	//return list of points - no direction needed,
	//draw funcion will change points into small squares
	//tadaaaa
}

