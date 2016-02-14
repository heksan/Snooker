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

int main(void)
{
	std::list<Ball*> listOfStationaryBalls;//not used
	std::list<Ball*> listOfBalls;

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
	

	//floor buffers
	GLuint vertexbufferFloor;
	glGenBuffers(1, &vertexbufferFloor);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferFloor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_t), g_vertex_buffer_data_t, GL_STATIC_DRAW);
	GLuint colorbufferFloor;
	glGenBuffers(1, &colorbufferFloor);
	glBindBuffer(GL_ARRAY_BUFFER, colorbufferFloor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data_t), g_color_buffer_data_t, GL_STATIC_DRAW);



	////////////////do balls - buffers relocating to be fixed//////////////////////

	createBuffers(listOfBalls);

	//createBuffer(verySpecialBall);
	//createBuffer(cueBall);
	//createBuffer(justBall);
	//createBuffer(justAnballTwo);

	relocateMatrices(listOfBalls);

	//relocateMatrix(verySpecialBall);
	//relocateMatrix(cueBall);
	//relocateMatrix(justBall);
	//relocateMatrix(justAnballTwo);


	do{

		//std::cout << listOfBalls.size();//fix

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);


		//Main game loop
		if (!stable){
			stable = checkStable(listOfBalls);
			//cueBall.matrix = moveBall(cueBall);//fix speed vec - use as multi for movvec,//////////////////////////////////ref function to take ball
			moveBalls(listOfBalls);//change all list of balls return to void, its just a pointer list
			checkWallCollisions(listOfBalls);
			checkBallCollisions(listOfBalls);
		}
		else{
			cameraPosition = computeMovFromInput();
			initMovement(6.0f, cameraPosition, cueBall.ballPosition, cueBall.movementVector);//later force from gUI
		}

		computeCameraMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();

		

		drawBalls(listOfBalls, MatrixID, ViewMatrix, Projection);

		//drawBall(verySpecialBall, MatrixID, ViewMatrix, ProjectionMatrix);
		//drawBall(justBall, MatrixID, ViewMatrix, ProjectionMatrix);
		//drawBall(justAnballTwo, MatrixID, ViewMatrix, ProjectionMatrix);


		////////////floor////////////////
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glm::mat4 MVP_floor = ProjectionMatrix * ViewMatrix * glm::mat4(1.0);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP_floor[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferFloor);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbufferFloor);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);



		///////////////////////////////////////////////

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
//
/// cue stick
// fix controls(fixed ish)
// fix ball.cpp speghetti
//, 
//
// fix cleanup glDeleteBuffers(1, &cueBall.vertexbuffer);
// glDeleteBuffers(1, &cueBall.colorbuffer);
// glDeleteProgram(programID);
// glDeleteVertexArrays(1, &VertexArrayID);
//make different collor buffer datas depending on IDs
//
//delete pointers when  in pocket - oh right, make pockets ffs
//
//
//
//
//
//
//
//
//
