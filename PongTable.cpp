/*****************************************************************
*File    : PongTable.cpp
*Author  : Luis D. Hernandez
*Class   : COP 2001 201901 10410
*Purpose : implementation file for PongTable using pointers to 
*          modify values in pong game 
*****************************************************************/

#include "PongTable.h"

PongTable::PongTable() {
	Position ballCurrent = { BALL_START_X, BALL_START_Y };
	Position ballPrevious = { 0,0 };
	Position ballVelocity = { BALL_START_VEL_X, BALL_START_VEL_Y};
	ball = { BALL_HEIGHT, BALL_WIDTH, ballCurrent, ballPrevious, ballVelocity, true };

	//top wall 
	int topHeight = WALL_THICKNESS;
	int topWidth = SCREEN_WIDTH - (2 * SCREEN_OFFSET);
	Position topCurrent = {SCREEN_OFFSET, SCREEN_OFFSET};
	Position topPrevious = {0,0};
	Position topVelocity = { 0,0 };
	topWall = PongObject(topHeight, topWidth, topCurrent, topPrevious, topVelocity, false);

	// left wall
	int leftHeight = SCREEN_HEIGHT - (2 * SCREEN_OFFSET); 
	int leftWidth = WALL_THICKNESS;
	Position leftCurrent = { SCREEN_OFFSET, SCREEN_OFFSET };
	Position leftPrevious = { 0,0 };
	Position leftVelocity = { 0,0 };
	leftWall = PongObject(leftHeight, leftWidth, leftCurrent, leftPrevious, leftVelocity, false);

	//right wall
	int rightHeight = SCREEN_HEIGHT - (2 * SCREEN_OFFSET);
	int rightWidth = WALL_THICKNESS;
	Position rightCurrent = { SCREEN_WIDTH - SCREEN_OFFSET - WALL_THICKNESS, SCREEN_OFFSET };
	Position rightPrevious = { 0,0 };
	Position rightVelocity = { 0,0 };
	rightWall = PongObject(rightHeight, rightWidth, rightCurrent, rightPrevious, rightVelocity, false);

	//bottom wall
	int bottomHeight = WALL_THICKNESS;
	int bottomWidth = SCREEN_WIDTH - (2 * SCREEN_OFFSET);
	Position bottomCurrent = { SCREEN_OFFSET, SCREEN_HEIGHT - SCREEN_OFFSET - WALL_THICKNESS};
	Position bottomPrevious = { 0,0 };
	Position bottomVelocity = { 0,0 };
	bottomWall = PongObject(bottomHeight, bottomWidth, bottomCurrent, bottomPrevious, bottomVelocity, false);

	// ai paddle
	Position aiPaddlePosition;
	aiPaddlePosition.xValue = rightWall.getCurrent().xValue - PADDLE_OFFSET - PADDLE_WIDTH;
	aiPaddlePosition.yValue = (SCREEN_HEIGHT / 2) - (PADDLE_HEIGHT / 2);
	cpuPaddle = PongObject(PADDLE_HEIGHT, PADDLE_WIDTH, aiPaddlePosition, { 0,0 }, { 0,0 }, false);

	// player paddle
	Position playerPadPos;
	playerPadPos.xValue = leftWall.getCurrent().xValue + PADDLE_OFFSET;
	playerPadPos.yValue = (SCREEN_HEIGHT / 2) - (PADDLE_HEIGHT / 2);
	playersPaddle = PongObject(PADDLE_HEIGHT, PADDLE_WIDTH, playerPadPos, { 0,0 }, { 0,0 }, false);
};

PongObject *PongTable::getBall() {

	return &ball;
};

PongObject *PongTable::getTopWall() {

	return &topWall;
};

PongObject *PongTable::getLeftWall() {

	return &leftWall;
};

PongObject *PongTable::getRightWall() {

	return &rightWall;
};

PongObject *PongTable::getBottomWall() {

	return &bottomWall;
};

PongObject *PongTable::getComputerPaddle() {
	return &cpuPaddle;
}

PongObject *PongTable::getPlayersPaddle() {

	return &playersPaddle;
}
void PongTable:: render(HDC console, float lag) {

	// draw player paddle
	playersPaddle.render(console, lag);

	// draw aiPaddle
	cpuPaddle.render(console, lag);

	//draw the ball
	ball.render(console, lag);

	//draw walls
	topWall.render(console, 0);
	leftWall.render(console, 0);
	rightWall.render(console, 0);
	bottomWall.render(console, 0);


	return;
};

bool PongTable::collosions() {

	// end game 
	bool scoredPoint = false;

	//balls current position and velocity
	Position ballCurrent = ball.getCurrent();
	Position ballVelocity = ball.getVelocity();

	// check for collision with walls and paddles

	if (ball.intersects(&cpuPaddle)) {
		ballCurrent.xValue = cpuPaddle.getCurrent().xValue - ball.getWidth() - 1;
		ballVelocity.xValue *= -1;
		cpuPaddle.setIsDirty(true);
	}
	else if (ball.intersects(&playersPaddle)) {
		ballCurrent.xValue = playersPaddle.getCurrent().xValue + ball.getWidth() + 1;
		ballVelocity.xValue *= -1;
		playersPaddle.setIsDirty(true);
	}
	else if (ball.intersects(&leftWall)) {      //left wall
		ballCurrent.xValue = leftWall.getCurrent().xValue + leftWall.getWidth() + 1;
		ballVelocity.xValue *= -1;
		leftWall.setIsDirty(true);
		scoredPoint = true;
	}
	else if (ball.intersects(&rightWall)) {     //right wall
		ballCurrent.xValue = rightWall.getCurrent().xValue - ball.getWidth() - 1;
		ballVelocity.xValue *= (-1);
		rightWall.setIsDirty(true);
		scoredPoint = true;
	}

	if (ball.intersects(&topWall)) { //top wall
		ballCurrent.yValue = topWall.getCurrent().yValue + ball.getHeight() + 1;
		ballVelocity.yValue *= (-1);
		topWall.setIsDirty(true);
	}
	else if (ball.intersects(&bottomWall)) {   //bottom wall
		ballCurrent.yValue = bottomWall.getCurrent().yValue - ball.getHeight() - 1;
		ballVelocity.yValue *= (-1);
		bottomWall.setIsDirty(true);
	}
	
	ball.setCurrent(ballCurrent);
	ball.setVelocity(ballVelocity);

	Position aiPadCur = cpuPaddle.getCurrent();
	Position aiPadVel = cpuPaddle.getVelocity();

	if (cpuPaddle.intersects(&topWall)) {
	
		aiPadCur.yValue = topWall.getCurrent().yValue + topWall.getHeight() + 1;
		aiPadVel.yValue = 0.0;
		topWall.setIsDirty(true);

	}
	else if (cpuPaddle.intersects(&bottomWall)) {
		
		aiPadCur.yValue = bottomWall.getCurrent().yValue - PADDLE_HEIGHT - 1;
		aiPadVel.yValue = 0.0;
		bottomWall.setIsDirty(true);
	}

	cpuPaddle.setCurrent(aiPadCur);
	cpuPaddle.setVelocity(aiPadVel);

	Position currentPlayerPad = playersPaddle.getCurrent();
	Position velocityPlayerPad = playersPaddle.getVelocity();

	if (playersPaddle.intersects(&topWall)) {

		currentPlayerPad.yValue = topWall.getCurrent().yValue + topWall.getHeight() + 1;
		velocityPlayerPad.yValue = 0.0;
		topWall.setIsDirty(true);

	}
	else if (playersPaddle.intersects(&bottomWall)) {

		currentPlayerPad.yValue = bottomWall.getCurrent().yValue - PADDLE_HEIGHT - 1;
		velocityPlayerPad.yValue = 0.0;
		bottomWall.setIsDirty(true);
	}

	playersPaddle.setCurrent(currentPlayerPad);
	playersPaddle.setVelocity(velocityPlayerPad);



	return scoredPoint;
};

void PongTable::moveComputerPaddle() {

	Position ballCurrent = ball.getCurrent();
	Position aiPadPos = cpuPaddle.getCurrent();
	Position aiPadVel = cpuPaddle.getVelocity();

	float paddleTop = aiPadPos.yValue;
	float paddleBottom = paddleTop + PADDLE_HEIGHT;
	paddleTop += PADDLE_HEIGHT / 3;
	paddleBottom -= PADDLE_HEIGHT / 3;

	float ballTop = ballCurrent.yValue;
	float ballBottom = ballTop + BALL_HEIGHT;


	if (ballBottom < paddleTop) {
		aiPadVel.yValue -= .02;
	}
	else if (ballTop > paddleBottom) {
		aiPadVel.yValue += .02;
	}
	else {
		aiPadVel.yValue = 0.0;
	}
	
	cpuPaddle.setVelocity(aiPadVel);
};