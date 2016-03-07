#include "player.h"



Player::Player()
{
	ID = 0;
	points = 0;

}

Player::Player(int Id)
{
	ID = Id;
	points = 0;
	collisionCount = 0;
	pocketableBalls = 0;
	firstHitBallID = 0;

}
