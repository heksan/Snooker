#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "ball.h"
#include "cueStick.h"

glm::mat4 moveBall(Ball& ball);
void moveBalls(std::list<Ball*> listOfBalls);
void initMovement(float speedVec, glm::vec2 mouseRay, Ball& ball);
bool checkStable(std::list<Ball*> listOfBalls);
void checkStop(std::list<Ball*> listOfBalls);
bool checkStable(Ball ball);
void relocateMatrix(Ball& ball);
void relocateMatrices(std::list<Ball*> listOfBalls);
void checkBallCollisions(std::list<Ball*> listOfBalls);
void checkWallCollisions(std::list<Ball*> listOfBalls);
void collideOneMoving(Ball* ballOne, Ball* ballTwo);
void collideStationary(Ball* ballOne, Ball* ballTwo);
void collideMoving(Ball *ballOne, Ball  *ballTwo);
void changeDirection(Ball* ball, char xORz);
void recalculateDeceleration(Ball* ball);
void relocateCueStick(glm::vec2 mouseRay,CueStick& cueStick,Ball ball);
void moveCueStick(CueStick& cueStick,float force,bool& ballsMoving,bool& cueStickMoving);
void moveStickToOrigin(CueStick& cueStick);

#endif
