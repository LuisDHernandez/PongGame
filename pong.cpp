/*****************************************************************
*File    : pong.cpp
*Author  : Luis D. Hernandez
*Class   : COP 2001 201901 10410
*Purpose : main application file for pong game
*****************************************************************/

#include <iostream>
#include <time.h>
#include "pong.h" // main application
#include "PongTable.h" // pong table game object
using namespace std;

//function prototypes 
Input playerInput();
bool processPlayerInput(PongTable &table);
bool update(PongTable &table);
HDC initConsole();

int main() {

	/* initialize console window and get a handle
	to its device context for drawing*/
	HDC hdc = initConsole();

	/****************************************************************/
	/* declare and initialize time
	components for frame calculations */
	LARGE_INTEGER frequency;
	LARGE_INTEGER lastTime;
	LARGE_INTEGER currentTime;
	LARGE_INTEGER elapsedTime;
	float deltaTime = 0;

	//get cpu clock freq
	QueryPerformanceFrequency(&frequency);

	// set starting time to now
	QueryPerformanceCounter(&currentTime);
	// set inital last time to starting time
	lastTime = currentTime;
	/****************************************************************/

	PongTable table = PongTable();

	//call ball drawing function 
	table.render(hdc, 0);

	// bool to decide game state
	bool goPlay = !(update(table));

	while (goPlay)	// not exit
	{
		// get current frame times
		QueryPerformanceCounter(&currentTime);
		//get time elapsed between last frame and start of this one
		elapsedTime.QuadPart = currentTime.QuadPart - lastTime.QuadPart;
		//set last frame time to current
		lastTime = currentTime;
		//add delta the current elapsed divided by cpu freq
		deltaTime += (double)elapsedTime.QuadPart * 1000 / (double)frequency.QuadPart;

		// bool value for end game loop
		goPlay = !processPlayerInput(table);

		// call for update function in while loop testing time
		while (deltaTime >= FRAME_RATE) {

			goPlay = !(update(table));

			deltaTime -= FRAME_RATE;
		}
		// call for draw to draw ball
		table.render(hdc, deltaTime / FRAME_RATE);

	} // end while not exit loop

	cin.ignore();

	return 0;
}


/************************************************************************************
* processes user inputs
* @param PongObject processed - takes in game object struct as in/out parameters
* @returns isProcessed - for as long as goPlay is true
************************************************************************************/

bool processPlayerInput(PongTable &table) {

	// get user input

	PongObject *playersPaddle = table.getPlayersPaddle();

	Input input = playerInput();

	bool isProcessed = false;

	Position playerPadVel = playersPaddle->getVelocity();

	// which direction to move ball
	switch (input)
	{
	case UP:
		playerPadVel.yValue -= .15;
		break;
	case DOWN:
		playerPadVel.yValue += .15;
		break;
	//case LEFT:
		//playerPadVel.xValue -= .25;
		//break;
	//case RIGHT:
	//	playerPadVel.xValue += .25;
	//	break;
	case QUIT:
		isProcessed = true;
		break;
	case NONE:
		break;
	}

	playersPaddle->setVelocity(playerPadVel);
	table.moveComputerPaddle();

	return isProcessed;
}




/***************************************************************************************
* Takes player input to move ball in a certain direction
* @returns Input	- direction the user inputted on keyboard and turn it into movement
***************************************************************************************/
Input playerInput()
{
	Input input = NONE;	// return direction to move

						//define key constants for WASD (hex literals)
	const WORD VK_W = 0x57;
	const WORD VK_A = 0x41;
	const WORD VK_S = 0x53;
	const WORD VK_D = 0x44;
	const WORD VK_X = 0x58;

	//Get handle to console standard input stream
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

	INPUT_RECORD eventBuff[128];				// event buffer
	unsigned long eventCount, eventsRead;

	// see if there are any events on the standard input stream
	GetNumberOfConsoleInputEvents(hin, &eventCount);

	if (eventCount > 0)
	{
		// read all events availabe into an array buffer
		ReadConsoleInput(
			hin,
			eventBuff,
			128,
			&eventsRead);

		/* see if any of the events read are keyboard events
		and specifically one of the keys we care about */
		unsigned long i = 0;
		while (input == NONE && i < eventsRead)
		{
			if (eventBuff[i].EventType == KEY_EVENT)
			{
				switch (eventBuff[i].Event.KeyEvent.wVirtualKeyCode)
				{
				case VK_W:		// player hit W key
				case VK_UP:		// or up arrow
					input = UP;	// move up
					break;

				case VK_S:		// player hit S key
				case VK_DOWN:	// or down arrow
					input = DOWN;  // move down
					break;

				case VK_A:		// player hit A key
				case VK_LEFT:	// or left arrow
					input = LEFT;  // move left
					break;

				case VK_D:		// player hit D key
				case VK_RIGHT:	// or right arrow
					input = RIGHT;  // move right
					break;

				case VK_X:		// player hit X key
					input = QUIT;	// exit game
					break;

				default:		// some other key was pressed
					input = NONE;
				} //end case statement

			} // end keyboard event type

			i++;
		} // end while events exist

	}

	// get rid of any other input in the stream
	FlushConsoleInputBuffer(hin);

	return input;
}

/***************************************************************************************
* updates the game state making the code more readable in main
* by keeping up with the positions and velocities and input
* @param PongObject ballandpaddle - current and last x and y positions
*       and the x and y velocties
* @return
***************************************************************************************/
bool update(PongTable &table) {

	PongObject *ball = table.getBall();
	PongObject *computerPaddle = table.getComputerPaddle();
	PongObject *playersPaddle = table.getPlayersPaddle();

	Position current = ball->getCurrent();
	Position velocity = ball->getVelocity();

	// add velocities to positions
	current.xValue += velocity.xValue;
	current.yValue += velocity.yValue;

	ball->setCurrent(current);

	//ai paddle
	Position aiPadCur = computerPaddle->getCurrent();
	Position aiPadVel = computerPaddle->getVelocity();
	aiPadCur.yValue += aiPadVel.yValue;

	computerPaddle->setCurrent(aiPadCur);

	Position playerCur = playersPaddle->getCurrent();
	Position playerVel = playersPaddle->getVelocity();
	playerCur.yValue += playerVel.yValue;
	playersPaddle->setCurrent(playerCur);

	//game over
	bool matchPoint = false;

	matchPoint = table.collosions();

	return matchPoint;
}

/**********************************************************
* Initializes the console window for drawing, and returns
* a handle to a drawing context for the console.
* @returns HDC	- handle to console's device context
**********************************************************/
HDC initConsole()
{
	//Get a handle to console window
	HWND conWindow = GetConsoleWindow();

	//Get handle to console standard output stream
	HANDLE conOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//Get a handle to a device context for drawing on the console
	HDC conDc = GetDC(conWindow);

	// resize the console window
	RECT conDimensions;
	GetWindowRect(conWindow, &conDimensions);
	MoveWindow(conWindow, conDimensions.left, conDimensions.top, SCREEN_WIDTH + 48, SCREEN_HEIGHT + 48, true);

	// turn off the blinking cursor
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1;
	cursor.bVisible = false;
	SetConsoleCursorInfo(conOut, &cursor);

	return conDc;
}