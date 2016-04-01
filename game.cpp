// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <vector>
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
#include "aimHelper.h"


std::vector<glm::vec3> helper;
GLFWwindow* window;
bool ballsMoving = false;
bool cueStickMoving = false;
bool foulCommited = true;
bool whitePocketed = true;
bool appropriateBallPocketed = false;
double speedVec = 1.0;
float force = 7.0f;

int main(void)
{

	//init matrices
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
	std::list<Ball*> listOfRepleacableBalls; 
	Ball cueBall(0, glm::vec3(0, 0, -130));

	/////////////////uncomment this and push_backs, delete test lines for all balls in right positions
	/*
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
	Ball redBall15(15, glm::vec3(-12, 0, 120));*/
	////////test line//////////////////
	Ball redBallX(1, glm::vec3(-80, 0, -170));
	/////////////////colour balls
	//16 yellow
	//17 orange
	//18 green
	//19blue
	//20 pink
	//21black
	
	
	//////////////////testline, uncomment line below to get right pos
	//Ball yellowBall(16, glm::vec3(80, 0, -170));
	Ball yellowBall(16, glm::vec3(30, 0, -100));
	Ball orangeBall(17, glm::vec3(0, 0, -100));
	Ball greenBall(18, glm::vec3(-30, 0, -100));
	Ball blueBall(19, glm::vec3(0, 0, 0));
	Ball pinkBall(20, glm::vec3(0, 0, 90));
	Ball blackBall(21, glm::vec3(0, 0, 150));

	listOfBalls.push_back(&cueBall);

	/*
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
	listOfBalls.push_back(&redBall15);*/


	////////test line//////////////////
	listOfBalls.push_back(&redBallX);
	//////////////////////////////
	
	listOfBalls.push_back(&yellowBall);
	listOfBalls.push_back(&orangeBall);
	listOfBalls.push_back(&greenBall);
	listOfBalls.push_back(&blueBall);
	listOfBalls.push_back(&pinkBall);
	listOfBalls.push_back(&blackBall);

	listOfRepleacableBalls.push_back(&yellowBall);
	listOfRepleacableBalls.push_back(&orangeBall);
	listOfRepleacableBalls.push_back(&greenBall);
	listOfRepleacableBalls.push_back(&blueBall);
	listOfRepleacableBalls.push_back(&pinkBall);
	listOfRepleacableBalls.push_back(&blackBall);
	listOfRepleacableBalls.push_back(&cueBall);

	//pockets
	std::list<Pocket*> listOfPockets;
	Pocket pocket1(glm::vec3(89.0f, 0.1f, 178.5f),-45);
	Pocket pocket2(glm::vec3(-89.0f, 0.1f, 178.5f),-135);
	Pocket pocket3(glm::vec3(89.0f, 0.1f, -178.5f), 45);
	Pocket pocket4(glm::vec3(-89.0f, 0.1f, -178.5f), 135);
	Pocket pocket5(glm::vec3(89.0f, 0.1f, 0.0f), 0);
	Pocket pocket6(glm::vec3(-89.0f, 0.1f, 0.0f), 180);

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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1024, 768, "Snooker Game", NULL, NULL);
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//prepare textures
	GLuint tableTexture = loadBMP(".\\texture.bmp");
	GLuint p1Texture = loadBMP(".\\player1.bmp");
	GLuint p2Texture = loadBMP(".\\player2.bmp");

	// background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	//vao, great mystery
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint colorShader = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
	GLuint textureShader = LoadShaders("TextureVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	GLuint textureShader2D = LoadShaders("TextureVertexShader2D.vertexshader", "TextureFragmentShader.fragmentshader");
	GLuint colorShader2D = LoadShaders("GUIVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");


	// uniforms
	GLuint MatrixID = glGetUniformLocation(colorShader, "MVP"); //apparently only this is used
	GLuint TextureMatrixID = glGetUniformLocation(textureShader, "MVP");
	GLuint SamplerID = glGetUniformLocation(textureShader, "textureSampler");

	createBuffer(table);
	createBuffer(cueStick);
	createBuffers(listOfBalls);
	createBuffers(listOfPockets);
	relocateMatrices(listOfBalls);
	relocateMatrices(listOfPockets);
	rotateMatrices(listOfPockets);
	
	//gui init
	initPowerBar2D();
	initPlayerIndicator2D();
	initAimHelper(colorShader);


	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		computeCameraMatricesFromInputs();
		ProjectionMatrix = getProjectionMatrix();//refactor this and next line
		ViewMatrix = getViewMatrix();

		//Main game loop//

		if (!ballsMoving && !cueStickMoving && !foulCommited){//stable
			appropriateBallPocketed = false;
			mouseRay = castRayThroughMouse();
			relocateCueStick(mouseRay, cueStick, cueBall);
			checkStart(cueStickMoving,force);
			initMovement(force, mouseRay, cueBall);//gives cueBall initial vectors 
			drawCueStick(cueStick, MatrixID, ViewMatrix, ProjectionMatrix);
			helper = calcAimHelper(mouseRay, cueBall, listOfBalls);
			drawAimHelper(helper, cueBall.ballPosition,MatrixID ,ViewMatrix, ProjectionMatrix);
			replacePocketedBalls(listOfBalls,listOfRepleacableBalls);
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
			if (!ballsMoving && !foulCommited && !appropriateBallPocketed){
				Player dummy = currentPlayer;
				currentPlayer = otherPlayer;
				otherPlayer = dummy;
				currentPlayerID = currentPlayer.ID;
				if (checkReds(listOfBalls)){
					resetPocketable(currentPlayer);
				}
				std::cout << "No ball pocketed, player " << currentPlayer.ID << " turn \n";
			}
			listOfBalls = checkWallCollisions(listOfBalls, foulCommited, whitePocketed, otherPlayer, currentPlayer,appropriateBallPocketed);
			moveBalls(listOfBalls);
			checkBallCollisions(listOfBalls,foulCommited,currentPlayer,otherPlayer);
		}
		if (foulCommited && !ballsMoving){
			appropriateBallPocketed = false;
			currentPlayerID = selectOtherPlayer(currentPlayer);//hold current player until next turn
			replacePocketedBalls(listOfBalls, listOfRepleacableBalls);
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
		//gui print
		glUseProgram(colorShader2D);
		printPowerBar2D(force);
		glUseProgram(textureShader2D);
		printPlayerIndicator2D(currentPlayerID, p1Texture, p2Texture);

		//game objects print
		glUseProgram(colorShader);
		drawBalls(listOfBalls, MatrixID, ViewMatrix, ProjectionMatrix);
		glUseProgram(textureShader);
		drawTable(table, TextureMatrixID, SamplerID, ViewMatrix, ProjectionMatrix,tableTexture);
		glUseProgram(colorShader);
		drawFrame(table, MatrixID, ViewMatrix, ProjectionMatrix);
		drawPockets(listOfPockets, MatrixID, ViewMatrix, ProjectionMatrix);

		glfwSwapBuffers(window);
		glfwPollEvents();
		glfwSwapInterval(1);//sets lags:D
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	cleanupBuffers(listOfBalls);
	cleanupBuffers(listOfPockets);
	glDeleteProgram(colorShader);
	glDeleteProgram(colorShader2D);
	glDeleteProgram(textureShader);
	glDeleteProgram(textureShader2D);
	glfwTerminate();
	return 0;
}



///////////////////////////notes

// fix two moving

/////////////// if enough time:
//angular velocity and angles(nope)
//mouse wheel
//
//
//
//
//
//
