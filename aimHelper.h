#ifndef AIMHELPER_H
#define AIMHELPER_H

#include "ball.h"
#include <list>
#include <vector>

void initAimHelper(GLuint shader);
void drawAimHelper(std::vector<glm::vec3> helper, glm::vec3 position, GLuint MatrixID , glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);
std::vector<glm::vec3> calcAimHelper(glm::vec2 mouseRay, Ball cueBall, std::list<Ball*> listOfBalls);//std::vector<glm::vec3>
bool checkCueCollision(Ball& cue, std::list<Ball*> listOfBalls);
void shoterWallCollision(Ball& ball);
#endif
