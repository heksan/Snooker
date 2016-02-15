// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;
bool stable = true;
double speedVec = 1.0;
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <shader.h>
#include <controls.h>
#include "gameEngine.h"
#include "graphicsEngine.h"
#include "ball.h"
#include "table.h"

int main(void)
{
	std::list<Ball*> listOfBalls;

	Table table;

	Ball cueBall(0, glm::vec3(0, 0, 0));
	Ball justBall(1, glm::vec3(40, 0, 40));
	Ball justAnballTwo(2, glm::vec3(20, 0, 20));
	Ball justAnballTwo2(3, glm::vec3(20, 0, 0));
	Ball justAnballTwo3(4, glm::vec3(0, 0, 20));
	Ball verySpecialBall(5, glm::vec3(60, 0, 60));

	listOfBalls.push_back(&cueBall);
	listOfBalls.push_back(&justBall);
	listOfBalls.push_back(&justAnballTwo);
	listOfBalls.push_back(&justAnballTwo2);
	listOfBalls.push_back(&justAnballTwo3);
	listOfBalls.push_back(&verySpecialBall);


	glm::vec3 rayOrigin;
	glm::vec3 rayDirection;
	glm::vec3 cameraPosition;



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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Snooker Game", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
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

	//GLuint MatrixID_t = glGetUniformLocation(programID, "MVP");

	// Projection matrix :60° Field of View, 4:3 ratio, display range : 0.1 unit <-> 500 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 500.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);


	///////init floor//////////////////////
	static const GLfloat g_vertex_buffer_data_t[] = {
		-89.0f, -2.6f, -178.5f,
		-89.0f, -2.6f, 178.5f,
		89.0f, -2.6f, 178.5f,
		-89.0f, -2.6f, -178.5f,
		89.0f, -2.6f, -178.5f,
		89.0f, -2.6f, 178.5f,
	};

	//floor colour
	static const GLfloat g_color_buffer_data_t[] = {
		0.4f, 1.0f, 0.4f,
		0.4f, 1.0f, 0.4f,
		0.4f, 1.0f, 0.4f,
		0.4f, 1.0f, 0.4f,
		0.4f, 1.0f, 0.4f,
		0.4f, 1.0f, 0.4f,
	};
	

	createBuffer(table);
	createBuffers(listOfBalls);


	relocateMatrices(listOfBalls);


	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);


		//Main game loop
		if (!stable){
			stable = checkStable(listOfBalls);
			moveBalls(listOfBalls);
			checkWallCollisions(listOfBalls);
			checkBallCollisions(listOfBalls);
			lookForCueStick();
		}
		else{
			cameraPosition = computeMovFromInput();

			initMovement(6.0f, cameraPosition, cueBall.ballPosition, cueBall.movementVector);//later force from gUI
		}

		computeCameraMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();

		drawBalls(listOfBalls, MatrixID, ViewMatrix, Projection);
		drawTable(table, MatrixID, ViewMatrix, Projection);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		glfwSwapInterval(1);
		

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader/////////////////////////////////////////////////fix
	glDeleteBuffers(1, &cueBall.vertexbuffer);
	glDeleteBuffers(1, &cueBall.colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}



//notes
//      REFACTOR BITCH
//
// change look at 
/// cue stick
// fix controls(fixed ish)
// fix ball.cpp speghetti
//, 
// ballmovement-x rahter than *0.xxx
// fix cleanup glDeleteBuffers(1, &cueBall.vertexbuffer);
// glDeleteBuffers(1, &cueBall.colorbuffer);
// glDeleteProgram(programID);
// glDeleteVertexArrays(1, &VertexArrayID);
//make different collor buffer datas depending on IDs
//
//delete pointers when  in pocket - oh right, make pockets ffs
//
//
//mouse wheel
//
//
//
//
//
//
