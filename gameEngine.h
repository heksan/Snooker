#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "ball.h"
#include "cueStick.h"
#include "pocket.h"

glm::mat4 moveBall(Ball& ball);
void moveBalls(std::list<Ball*> listOfBalls);
void initMovement(float speedVec, glm::vec2 mouseRay, Ball& ball);
bool checkStable(std::list<Ball*> listOfBalls);
void checkStop(std::list<Ball*> listOfBalls);
bool checkStable(Ball ball);
void relocateMatrix(Ball& ball);
void relocateMatrices(std::list<Ball*> listOfBalls);
void relocateMatrices(std::list<Pocket*> listOfPockets);
void checkBallCollisions(std::list<Ball*> listOfBalls);
std::list<Ball*> checkWallCollisions(std::list<Ball*> listOfBalls);
void collideOneMoving(Ball* ballOne, Ball* ballTwo);
void collideStationary(Ball* ballOne, Ball* ballTwo);
void collideMoving(Ball *ballOne, Ball  *ballTwo);
void changeDirection(Ball* ball, char xORz);
void recalculateDeceleration(Ball* ball);
void relocateCueStick(glm::vec2 mouseRay,CueStick& cueStick,Ball ball);
void moveCueStick(CueStick& cueStick,float force,bool& ballsMoving,bool& cueStickMoving);
void moveStickToOrigin(CueStick& cueStick);
std::list <Ball*> removeBall(std::list <Ball*> listOfBalls, Ball* ball);
void replacePocketedBalls(std::list<Ball*>& listOfBalls, std::list <Ball*> listOfRepleacableBalls);
void moveBallToStartingPosition(Ball *ballToBeInserted);
#endif
