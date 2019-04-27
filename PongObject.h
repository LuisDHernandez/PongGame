/*****************************************************************
*File    : PongObject.h
*Author  : Luis D. Hernandez
*Class   : COP 2001 201901 10410
*Purpose : Header file for pong game object such as ball, 
*		   paddles and wall
*****************************************************************/

#include "pong.h";

#ifndef PONGOBJECT_H
#define PONGOBJECT_H

class PongObject {

public:
	PongObject();

	PongObject(int heightIn, int widthIn, Position currentIn, Position previousIn, Position velocityIn, bool flagIn);

	Position getCurrent();
	Position getPrevious();
	Position getVelocity();
	Position getCurrent(float lag);

	void setCurrent(Position value);
	void setPrevious(Position value);
	void setVelocity(Position value);

	void erase(HDC console);
	void draw(HDC console, float lag);
	void render(HDC console, float lag);
	bool hasMoved(float lag);

	/*Accessor Methods*/
	int getHeight();
	int getWidth();
	bool isBall();
	bool isDirty();

	/*Setter Methods*/
	void setHeight(int value);
	void setWidth(int value);
	void setIsBall(bool flagvar);
	void setIsDirty(bool dirtyId);

	/*HELPER METHODS*/
	bool intersects(PongObject *other);


private:
	Position current;
	Position previous;
	Position velocity;
	int height;
	int width;
	bool ballFlag;
	bool dirty;

}; // end class PongObject

#endif //  PONGOBJECT_H
