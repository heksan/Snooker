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
#include "shader.h"
#include "controls.h"
#include "gameEngine.h"
#include "graphicsEngine.h"
#include "ball.h"
#include "table.h"
#include "cueStick.h"
#include "pocket.h"
#include "gui.h"
#include "player.h"


GLFWwindow* window;
bool ballsMoving = false;
bool cueStickMoving = false;
bool foulCommited = true;
bool whitePocketed = true;
double speedVec = 1.0;
float force = 7.0f;

int main(void)
{

	//init matrices
	glm::vec3 cameraPosition;//later will not used
	glm::vec2 mouseRay;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

	// players
	Player p1(1);
	Player p2(2);
	int currentPlayerID = p1.ID;
	Player currentPlayer = p2;
	Player otherPlayer = p1;

	//table and stick
	Table table;
	CueStick cueStick;

	//init balls and its list
	std::list<Ball*> listOfBalls;
	std::list<Ball*> listOfRePreacableBalls; //here goes white and all colours besides red
	Ball cueBall(0, glm::vec3(0, 0, -130));
	
	Ball redBall1(1, glm::vec3(0, 0, 96));
	Ball redBall2(2, glm::vec3(4, 0, 102));
	Ball redBall3(3, glm::vec3(-4, 0, 102));
	Ball redBall4(4, glm::vec3(0, 0, 108));
	Ball redBall5(5, glm::vec3(6, 0, 108));
	Ball redBall6(6, glm::vec3(-6, 0, 108));
	Ball redBall7(7, glm::vec3(4, 0, 114));
	Ball redBall8(8, glm::vec3(10, 0, 114));
	Ball redBall9(9, glm::vec3(-4, 0, 114));
	Ball redBall10(10, glm::vec3(-10, 0, 114));
	Ball redBall11(11, glm::vec3(0, 0, 120));
	Ball redBall12(12, glm::vec3(6, 0, 120));
	Ball redBall13(13, glm::vec3(12, 0, 120));
	Ball redBall14(14, glm::vec3(-6, 0, 120));
	Ball redBall15(15, glm::vec3(-12, 0, 120));


	/////////////////colour balls
	//16 yellow
	//17 orange
	//18 green
	//19blue
	//20 pink
	//21black
	Ball yellowBall(16, glm::vec3(30, 0, -100));
	Ball orangeBall(17, glm::vec3(0, 0, -100));
	Ball greenBall(18, glm::vec3(-30, 0, -100));
	Ball blueBall(19, glm::vec3(0, 0, 0));
	Ball pinkBall(20, glm::vec3(0, 0, 90));
	Ball blackBall(21, glm::vec3(0, 0, 150));

	listOfBalls.push_back(&cueBall);
	listOfBalls.push_back(&redBall1);
	listOfBalls.push_back(&redBall2);
	listOfBalls.push_back(&redBall3);
	listOfBalls.push_back(&redBall4);
	listOfBalls.push_back(&redBall5);
	listOfBalls.push_back(&redBall6);
	listOfBalls.push_back(&redBall7);
	listOfBalls.push_back(&redBall8);
	listOfBalls.push_back(&redBall9);
	listOfBalls.push_back(&redBall10);
	listOfBalls.push_back(&redBall11);
	listOfBalls.push_back(&redBall12);
	listOfBalls.push_back(&redBall13);
	listOfBalls.push_back(&redBall14);
	listOfBalls.push_back(&redBall15);
	listOfBalls.push_back(&yellowBall);
	listOfBalls.push_back(&orangeBall);
	listOfBalls.push_back(&greenBall);
	listOfBalls.push_back(&blueBall);
	listOfBalls.push_back(&pinkBall);
	listOfBalls.push_back(&blackBall);

	listOfRePreacableBalls.push_back(&cueBall);
	listOfRePreacableBalls.push_back(&yellowBall);
	listOfRePreacableBalls.push_back(&orangeBall);
	listOfRePreacableBalls.push_back(&greenBall);
	listOfRePreacableBalls.push_back(&blueBall);
	listOfRePreacableBalls.push_back(&pinkBall);
	listOfRePreacableBalls.push_back(&blackBall);

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
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);

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
	
	//test gui
	initPowerBar2D();
	initPlayerIndicator2D();



	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glUseProgram(programID);
		printPowerBar2D(force);
		printPlayerIndicator2D(currentPlayerID);
		glUseProgram(programID);
		computeCameraMatricesFromInputs();
		ProjectionMatrix = getProjectionMatrix();//refactor this and next line
		ViewMatrix = getViewMatrix();

		//Main game loop//

		if (!ballsMoving && !cueStickMoving && !foulCommited){//stable
			mouseRay = castRayThroughMouse();
			relocateCueStick(mouseRay, cueStick, cueBall);
			checkStart(cueStickMoving,force);
			initMovement(force, mouseRay, cueBall);//gives cueBall initial vectors 
			drawCueStick(cueStick, MatrixID, ViewMatrix, ProjectionMatrix);
			replacePocketedBalls(listOfBalls,listOfRePreacableBalls);
		}
		if (cueStickMoving){
			drawCueStick(cueStick, MatrixID, ViewMatrix, ProjectionMatrix);
			moveCueStick(cueStick, force, ballsMoving, cueStickMoving);//when finished sets cueStickMoving to false and ballsMoving to true
		}
		if (ballsMoving){
			checkStop(listOfBalls);
			ballsMoving = checkStable(listOfBalls); // sets ballsMoving to false if no ball moves
			if (!ballsMoving && !whitePocketed){
				checkCollisionCount(foulCommited, currentPlayer,otherPlayer);
			}
			if (!ballsMoving && !foulCommited){
				Player dummy = currentPlayer;
				currentPlayer = otherPlayer;
				otherPlayer = dummy;
				currentPlayerID = currentPlayer.ID;
				std::cout << "No ball pocketed, player " << currentPlayer.ID << " turn \n";
			}
			listOfBalls = checkWallCollisions(listOfBalls, foulCommited, whitePocketed, otherPlayer, currentPlayer);
			moveBalls(listOfBalls);
			checkBallCollisions(listOfBalls,foulCommited,currentPlayer,otherPlayer);
		}
		if (foulCommited && !ballsMoving){
			currentPlayerID = selectOtherPlayer(currentPlayer);//hold current player until next turn
			replacePocketedBalls(listOfBalls, listOfRePreacableBalls);
			mouseRay = castRayThroughMouse();
			if (whitePocketed){
				relocateCueBall(mouseRay, cueBall);
				checkClick(foulCommited);
				if (foulCommited == false){

					//switch players, move to method
					Player dummy = currentPlayer;
					currentPlayer = otherPlayer;
					otherPlayer = dummy;

					whitePocketed = false;
					std::cout << "player " << currentPlayer.ID << " turn \n";
				}
			}
			else{
				foulCommited = false;

				Player dummy = currentPlayer;
				currentPlayer = otherPlayer;
				otherPlayer = dummy;

				std::cout << "player " << currentPlayer.ID << " turn \n";
			}
		}


		////end of game loop//////
		
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



///////////////////////////notes
//comment game, engine
//set same order for players in all methods
// change orange to brown and swap
// ref shader
// fix two moving
//gui - new shader needed for text
//trace
//


//points for first hit or no hit

//repleacable not repleacable when all red pocketed
//
// table can be static?
///
// 
// graphics should be better
//
//
//
/////////////// if enough time:
//angular velocity
//mouse wheel
//
//
//
//
//
//
