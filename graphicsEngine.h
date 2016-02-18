#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include "ball.h"
#include "table.h"
#include "cueStick.h"

void createBuffer(Table& table);
void createBuffer(CueStick& cueStick);
void createBuffer(Ball& ball);
void createBuffers(std::list<Ball*> listOfBalls);
void drawBall(Ball ball, GLuint MatrixID, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);
void drawBalls(std::list<Ball*> listOfBalls, GLuint MatrixID, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);
void drawTable(Table table, GLuint MatrixID, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);
void drawCueStick(CueStick cueStick, GLuint MatrixID, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);
void cleanupBuffers(std::list<Ball*> listOfBalls);
#endif