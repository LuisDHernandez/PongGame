/*****************************************************************
*File    : pong.h
*Author  : Luis D. Hernandez
*Class   : COP 2001 201901 10410
*Purpose : Header file for main application file for pong game
*****************************************************************/

#include <Windows.h> // console graphic

#ifndef PONG_H
#define PONG_H

// key input directions
enum Input {
	QUIT,
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// position coordinate structure 
struct Position {
	float xValue;
	float yValue;
};

//Update frame rate in milliseconds
const float FRAME_RATE = 1000 / 60; // = 16.666666

// ball dimensions
const int BALL_WIDTH = 6;
const int BALL_HEIGHT = 6;

//screen dimensions constants 
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 256;

// define color constants
const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF PAINT = WHITE;

// screen dimension constants
const int SCREEN_OFFSET = 2;

//wall dimension constants
const int WALL_THICKNESS = 1;

// paddle dimension constants 
const int PADDLE_HEIGHT = 28;
const int PADDLE_WIDTH = 2;
const int PADDLE_OFFSET = 4;

//ball dimension constants
const int BALL_START_X = SCREEN_OFFSET + PADDLE_OFFSET + PADDLE_WIDTH + 10;
const int BALL_START_Y = (SCREEN_HEIGHT / 2) - 10;
const int BALL_START_VEL_X = -2;
const int BALL_START_VEL_Y = 1;


//the ball as an array
const COLORREF BALL_PATTERN[BALL_HEIGHT][BALL_WIDTH] = {
	{ BLACK, BLACK, PAINT, PAINT, BLACK, BLACK },
{ BLACK, PAINT, PAINT, PAINT, PAINT, BLACK },
{ PAINT, PAINT, PAINT, PAINT, PAINT, PAINT },
{ PAINT, PAINT, PAINT, PAINT, PAINT, PAINT },
{ BLACK, PAINT, PAINT, PAINT, PAINT, BLACK },
{ BLACK, BLACK, PAINT, PAINT, BLACK, BLACK } };


#endif //  PONG_H