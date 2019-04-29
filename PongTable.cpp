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

	//wallNet aka middle wall extra credit 
	int wallNetHeight = SCREEN_HEIGHT - SCREEN_OFFSET - WALL_THICKNESS;
	int wallNetWidth = WALL_THICKNESS;
	Position netCurrent = {SCREEN_WIDTH / 2, 0 };
	Position netPrevious = {0,0};
	Position netVelocity = {0,0};
	wallNet = PongObject(wallNetHeight,wallNetWidth,netCurrent,netPrevious,netVelocity,false);

	//serveLine wall
	int serveLineHeight = WALL_THICKNESS;
	int serveLineWidth = SCREEN_WIDTH - (SCREEN_HEIGHT / 2);
	Position serveLineCur = {SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2};
	Position serveLinePrev = {0,0};
	Position serveLineVel = { 0,0 };
	serveLine = PongObject(serveLineHeight, serveLineWidth, serveLineCur, serveLinePrev, serveLineVel, false);

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

PongObject *PongTable::getWallNet() {

	return &wallNet;
}

PongObject *PongTable::getServeLine() {

	return &serveLine;

}

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

	//draw serve and net lines
	wallNet.render(console, 0);
	serveLine.render(console, 0);

	return;
};

bool PongTable::collosions() {

	// end game 
	bool scoredPoint = false;

	//balls current position and velocity
	Position ballCurrent = ball.getCurrent();
	Position ballVelocity = ball.getVelocity();

	// check for collision with walls and paddles

	if (ball.intersects(&cpuPaddle)) { // cpupaddle intersection

		ballCurrent.xValue = cpuPaddle.getCurrent().xValue - ball.getWidth() - 1;
		ballVelocity = hitTrickshot(cpuPaddle);
		ballVelocity.xValue *= -1;
		cpuPaddle.setIsDirty(true);
	}
	else if (ball.intersects(&playersPaddle)) {    //player paddle

		ballCurrent.xValue = playersPaddle.getCurrent().xValue + ball.getWidth() + 1;
		ballVelocity = hitTrickshot(playersPaddle);
		ballVelocity.xValue *= -1;
		playersPaddle.setIsDirty(true);
	}
	else if (ball.intersects(&leftWall)) {   //left wall

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

	if (ball.intersects(&wallNet)) { // middle wall

		wallNet.setIsDirty(true);
	}
	if (ball.intersects(&serveLine)) { // serveline

		serveLine.setIsDirty(true);
	}
	
	ball.setCurrent(ballCurrent);
	ball.setVelocity(ballVelocity);

	//ai paddle wall interactions
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

	//player paddle wall interactions
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
	// conditional check for max ai velocity extra credit
	if (aiPadVel.yValue > MAX_CPU_PADDLE_VELOCITY) {
	
		aiPadVel.yValue = MAX_CPU_PADDLE_VELOCITY;
	}
	else if (-(aiPadVel.yValue) < -(MAX_CPU_PADDLE_VELOCITY)) {
	
		aiPadVel.yValue = -(MAX_CPU_PADDLE_VELOCITY);
	}
	cpuPaddle.setVelocity(aiPadVel);
};

Position PongTable::hitTrickshot(PongObject somePaddle) { // extra credit trickshot function

	//get ball velocity to calculate trickshot
	Position trickshotVelocity = ball.getVelocity();

	// bool variables to test situations
	bool isUp = trickshotVelocity.yValue < 0 ;
	bool isDown = trickshotVelocity.yValue > 0;
	bool isRight = trickshotVelocity.xValue > 0;
	bool isLeft = trickshotVelocity.xValue < 0;

	// local paddle division for bool check
	float padTop = somePaddle.getCurrent().yValue;
	float padBot = somePaddle.getCurrent().yValue + PADDLE_HEIGHT;
	padTop += PADDLE_HEIGHT / 3;
	padBot -= PADDLE_HEIGHT / 3;

	//call ball midpoint to test if its hitting top or bottom of paddle
	float ballMidpoint = ball.getCurrent().yValue + BALL_HEIGHT / 2;

	bool isTop = ballMidpoint > padTop;
	bool isBottom = ballMidpoint < padBot;

	if ( isTop && isUp ) {
	
		trickshotVelocity.yValue -= 0.05;

		if (isLeft) {
		
			trickshotVelocity.xValue -= .25;
		}
		else if (isRight) {
		
			trickshotVelocity.xValue += .25;
		}
	}

	else if ( isBottom && isUp) {
	
		trickshotVelocity.yValue += .25;

		if (isLeft) {
		
			trickshotVelocity.xValue += .25;
		}
		else if (isRight) {
		
			trickshotVelocity.xValue -= .25;
		}
	}
	else if ( isTop && !(isUp)) {
	
		trickshotVelocity.yValue -= .25;

		if(isLeft) {
		
			trickshotVelocity.xValue += .25;
		}
		else if (isRight) {
		
			trickshotVelocity.xValue -= .25;
		}

	}
	else if ( isBottom && !(isUp)) {
	
		trickshotVelocity.yValue += .5;
		
		if (isLeft) {

			trickshotVelocity.xValue -= .25;
		}
		else if (isRight) {
		
			trickshotVelocity.xValue += .25;
		}
	}

	return trickshotVelocity;

}