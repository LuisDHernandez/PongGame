/*****************************************************************
*File    : PongObject.cpp
*Author  : Luis D. Hernandez
*Class   : COP 2001 201901 10410
*Purpose : implementation file for pong game object such as ball,
*		   paddles and wall
*****************************************************************/

#include "PongObject.h"

/**/
PongObject::PongObject() {
	height = 0;
	width = 0;
	current = { 0.0f ,0.0f };
	previous = { 0.0f ,0.0f };
	velocity = { 0.0f ,0.0f };
	dirty = true;
}

/**/
PongObject::PongObject(int heightIn, int widthIn, Position currentIn, Position previousIn, Position velocityIn, bool flagIn) {

	current = currentIn;
	previous = previousIn;
	velocity = velocityIn;
	height = heightIn;
	width = widthIn;
	ballFlag = flagIn;
	dirty = true;

}

/***ACCESSOR METHODS***/
Position PongObject::getCurrent() { return current; }
Position PongObject::getPrevious() { return previous; }
Position PongObject::getVelocity() { return velocity; }
int PongObject::getHeight() { return height; }
int PongObject::getWidth() { return width; }
bool PongObject::isBall() { return ballFlag; }
bool PongObject::isDirty() { return dirty; }

Position PongObject::getCurrent(float lagtime) {

	Position localValue;

	localValue.xValue = current.xValue + (lagtime * velocity.xValue);
	localValue.yValue = current.yValue + (lagtime * velocity.yValue);

	return localValue;
}

/***SETTER METHODS***/
void PongObject::setCurrent(Position value) {
	current = value;
}
void PongObject::setPrevious(Position value) {
	previous = value;
}
void PongObject::setVelocity(Position value) {
	velocity = value;
}
void PongObject::setHeight(int heightIn) {
	height = heightIn;
}
void PongObject::setWidth(int widthIn) {
	width = widthIn;
}
void PongObject::setIsBall(bool flagIn) {
	ballFlag = flagIn;
}
void PongObject::setIsDirty(bool dirtyId) {
	dirty = dirtyId;
}


/***HELPER METHOD***/

bool PongObject::hasMoved(float lag) {

	Position ballChange;

	ballChange = getCurrent(lag);

	bool movement = int(ballChange.xValue) != int(previous.xValue) || int(ballChange.yValue) != int(previous.yValue);

	return movement;

}

void PongObject::erase(HDC console) {

	int xpos = previous.xValue;
	int ypos = previous.yValue;

	for (int row = 0; row <= height; row++) {
		for (int col = 0; col <= width; col++) {
			SetPixel(console, xpos + col, ypos + row, BLACK);
		}
	}

	return;
}

void PongObject::draw(HDC console, float lag) {

	Position adjusted = getCurrent(lag);

	int xpos = adjusted.xValue;
	int ypos = adjusted.yValue;
	
	for (int row = 0; row < height ; row++) {
		for (int col = 0; col < width; col++) {
			COLORREF paint = PAINT;
			if (isBall()) {
				paint = BALL_PATTERN[col][row];
			}
			SetPixel(console, xpos + col, ypos + row, paint);
		}
	}
	setPrevious(adjusted);

	return;
}

void PongObject::render(HDC console, float lag) {

	if (hasMoved(lag) || isDirty()) {
		erase(console);
		draw(console, lag);
		setIsDirty(false);
	}

}

bool PongObject::intersects(PongObject *other) {
	// local object
	float thisTop = current.yValue;
	float thisBottom = current.yValue + height;
	float thisLeft = current.xValue;
	float thisRight = current.xValue + width;

	//other object intersecting thisObject
	float otherTop = other->getCurrent().yValue;
	float otherBottom = other->getCurrent().yValue + other->getHeight();
	float otherLeft = other->getCurrent().xValue;
	float otherRight = other->getCurrent().xValue + other->getWidth();

	return !(otherLeft > thisRight
		|| otherRight < thisLeft
		|| otherTop > thisBottom
		|| otherBottom < thisTop);
};