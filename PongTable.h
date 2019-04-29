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

	/*ACCESSOR METHODS*/
	PongObject *getBall();
	PongObject *getTopWall();
	PongObject *getLeftWall();
	PongObject *getRightWall();
	PongObject *getBottomWall();
	PongObject *getWallNet();
	PongObject *getServeLine();
	PongObject *getComputerPaddle();
	PongObject *getPlayersPaddle();

	/*HELPER METHODS*/
	void render(HDC console, float lag);
	bool collosions();
	void moveComputerPaddle();
	
private:

	PongObject ball;
	PongObject topWall;
	PongObject leftWall;
	PongObject rightWall;
	PongObject bottomWall;
	PongObject wallNet;
	PongObject serveLine;
	PongObject cpuPaddle;
	PongObject playersPaddle;
	Position hitTrickshot(PongObject somePaddle);

};// end PongTable
#endif // !PONGTABLE_H 