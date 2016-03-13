#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "ball.h"
#include "cueStick.h"
#include "pocket.h"
#include "player.h"

//moving, slowing down
glm::mat4 moveBall(Ball& ball);
void initMovement(float speedVec, glm::vec2 mouseRay, Ball& ball);
glm::mat4 moveBall(Ball& ball);
void moveBalls(std::list<Ball*> listOfBalls);
void recalculateDeceleration(Ball* ball);

//stopiing ball, checking if all balls have stopped
bool checkStable(std::list<Ball*> listOfBalls);
void checkStop(std::list<Ball*> listOfBalls);
bool checkStable(Ball ball);

//Moving meshes
void relocateMatrix(Ball& ball);
void relocateMatrices(std::list<Ball*> listOfBalls);
void relocateMatrices(std::list<Pocket*> listOfPockets);

//ball to ball collisions
void checkBallCollisions(std::list<Ball*> listOfBalls, bool& foulCommitted, Player& currentPlayer, Player& otherPlayer);
void collideOneMoving(Ball* ballOne, Ball* ballTwo);
void collideStationary(Ball* ballOne, Ball* ballTwo);
void collideMoving(Ball *ballOne, Ball  *ballTwo);

//ball to wall collisions
std::list<Ball*> checkWallCollisions(std::list<Ball*> listOfBalls, bool& foulCommited,bool& whitePocketed, Player& otherPlayer, Player& currentPlayer);
void traceBackToWallContackPoint(std::list<Ball*>::iterator ball);
void changeDirection(Ball* ball, char xORz);

//cueStick Movement
void relocateCueStick(glm::vec2 mouseRay,CueStick& cueStick,Ball ball);
void moveCueStick(CueStick& cueStick,float force,bool& ballsMoving,bool& cueStickMoving);
void moveStickToOrigin(CueStick& cueStick);

// methods used after pocketing a ball
std::list <Ball*> removeBall(std::list <Ball*> listOfBalls, Ball* ball);
void replacePocketedBalls(std::list<Ball*>& listOfBalls, std::list<Ball*>& listOfRepleacableBalls);
void moveBallToStartingPosition(Ball *ballToBeInserted);
void relocateCueBall(glm::vec2 mouseRay, Ball& cueball);

//players
int selectOtherPlayer(Player currentPlayer);

//points, fouls and game rules
void decidePointsAndFoulsPockets(int ballID, bool& foulCommited, bool& whitePocketed, Player& otherPlayer, Player& currentPlayer);
void resetPocketable(Player& player);
void changePocketable(Player& currentPlayer);
void givePoints(int points, Player& player);
int decidePoints(int ballID);
void foulOnFirstCollision(int ballID, bool& foulCommitted, Player& currentPlayer, Player& otherPlayer);
void checkCollisionCount(bool& foulCommited,Player& currentPlayer,Player& otherPlayer);
#endif
