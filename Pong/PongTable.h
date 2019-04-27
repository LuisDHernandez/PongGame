/*****************************************************************
*File    : PongTable.h
*Author  : Luis D. Hernandez
*Class   : COP 2001 201901 10410
*Purpose : Header file for pong table that will be used for 
*          for pointers for the game objects
*****************************************************************/

#include "PongObject.h"

#ifndef PONGTABLE_H
#define PONGTABLE_H

class PongTable {

public:
	PongTable();
	PongObject *getBall();
	PongObject *getTopWall();
	PongObject *getLeftWall();
	PongObject *getRightWall();
	PongObject *getBottomWall();
	void render(HDC console, float lag);
	bool collosions();
	PongObject *getComputerPaddle();
	void moveComputerPaddle();
	PongObject *getPlayersPaddle();
private:
	PongObject ball;
	PongObject topWall;
	PongObject leftWall;
	PongObject rightWall;
	PongObject bottomWall;
	PongObject computerPaddle;
	PongObject playersPaddle;
	
};// end PongTable
#endif // !PONGTABLE_H 