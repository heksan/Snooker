#ifndef PLAYER_H
#define PLAYER_H






class Player
{

public:
	int points;
	int ID;
	int pocketableBalls; // 0 for reds ,1 for cols
	int collisionCount;
	Player();
	Player(int ID);

};



#endif