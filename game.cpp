// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

//include everything else
#include <shader.h>
#include <controls.h>
#include "gameEngine.h"
#include "graphicsEngine.h"
#include "ball.h"
#include "table.h"
#include "cueStick.h"
#include "pocket.h"


GLFWwindow* window;
bool ballsMoving = false;
bool cueStickMoving = false;
double speedVec = 1.0;


int main(void)
{
	//init matrices
	glm::vec3 cameraPosition;//later will not used
	glm::vec2 mouseRay;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;


	//table and stick
	Table table;
	CueStick cueStick;

	//init balls and its list
	std::list<Ball*> listOfBalls;
	Ball cueBall(0, glm::vec3(0, 0, 0));
	Ball justBall(1, glm::vec3(6, 0, 6));
	Ball justAnballTwo(2, glm::vec3(12, 0, 12));
	//Ball justAnballTwo2(3, glm::vec3(18, 0, 18));
	//Ball justAnballTwo3(4, glm::vec3(20, 0, 30));
	//Ball justAnballTwo4(5, glm::vec3(20, 0,20));
	//Ball justAnballTwo5(6, glm::vec3(0, 0, 20));
	//Ball justAnballTwo6(7, glm::vec3(20, 0, 0));
	//Ball justAnballTwo7(8, glm::vec3(0, 0, -20));
	//Ball verySpecialBall(9, glm::vec3(60, 0, 60));

	listOfBalls.push_back(&cueBall);
	listOfBalls.push_back(&justBall);
	listOfBalls.push_back(&justAnballTwo);
	//listOfBalls.push_back(&justAnballTwo2);
	//listOfBalls.push_back(&justAnballTwo3);
	//listOfBalls.push_back(&verySpecialBall);

	//pockets
	std::list<Pocket*> listOfPockets;
	Pocket pocket1(glm::vec3(89.0f, 0.1f, 178.5f));
	Pocket pocket2(glm::vec3(-89.0f, 0.1f, 178.5f));
	Pocket pocket3(glm::vec3(89.0f, 0.1f, -178.5f));
	Pocket pocket4(glm::vec3(-89.0f, 0.1f, -178.5f));
	Pocket pocket5(glm::vec3(89.0f, 0.1f, 0.0f));
	Pocket pocket6(glm::vec3(-89.0f, 0.1f, 0.0f));

	listOfPockets.push_back(&pocket1);
	listOfPockets.push_back(&pocket2);
	listOfPockets.push_back(&pocket3);
	listOfPockets.push_back(&pocket4);
	listOfPockets.push_back(&pocket5);
	listOfPockets.push_back(&pocket6);
	
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1024, 768, "Snooker Game", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP"); //apparently only this is used

	createBuffer(table);
	createBuffer(cueStick);
	createBuffers(listOfBalls);
	createBuffers(listOfPockets);
	relocateMatrices(listOfBalls);
	relocateMatrices(listOfPockets);
	

	



	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		computeCameraMatricesFromInputs();
		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();


		//Main game loop//
		if (!ballsMoving && !cueStickMoving){
			mouseRay = castRayThroughMouse();
			relocateCueStick(mouseRay, cueStick, cueBall);
			checkStart(cueStickMoving);
			initMovement(6.0f, mouseRay, cueBall);//gives cueBall initial vectors 
			drawCueStick(cueStick, MatrixID, ViewMatrix, ProjectionMatrix);
		}
		if (cueStickMoving){
			drawCueStick(cueStick, MatrixID, ViewMatrix, ProjectionMatrix);
			moveCueStick(cueStick, 10.0f, ballsMoving, cueStickMoving);//when finished sets cueStickMoving to false and ballsMoving to true
		}
		if (ballsMoving){
			checkStop(listOfBalls);
			ballsMoving = checkStable(listOfBalls);
			checkWallCollisions(listOfBalls);
			checkBallCollisions(listOfBalls);
			moveBalls(listOfBalls);
			
		}
		

		////end//////
		
		
		drawBalls(listOfBalls, MatrixID, ViewMatrix, ProjectionMatrix);
		drawTable(table, MatrixID, ViewMatrix, ProjectionMatrix);
		drawPockets(listOfPockets, MatrixID, ViewMatrix, ProjectionMatrix);

		glfwSwapBuffers(window);
		glfwPollEvents();
		glfwSwapInterval(1);


	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);


	cleanupBuffers(listOfBalls);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
	return 0;
}



//notes
//      REFACTOR BITCH
//
//
//
//
// table can be static?
///
// fix controls
// close balls dirs
//
//make different collor buffer datas depending on IDs
//
//delete pointers when  in pocket - oh right, make pockets ffs
//
//angular velocity
//mouse wheel
//
//
//
//
//
//
