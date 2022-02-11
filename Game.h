#ifndef GAME_H
#define GAME_H

// Include
#include <Windows.h>
#include "GameObjects.h"

class Game
{
public:
	// Functions
	Game();
	void Initialise();
	void Update(float deltaTime);
	void Draw();

private:
	// ENUMS
	enum GAME_STATE
	{
		SPLASH,
		MENU,
		PLAY,
		SCOREBOARD
	};

	// Console variables (change comments)
	// Windows size coordinates 
	SMALL_RECT windowSize = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	//  A COORD struct for specifying the console's sccreen bufer dimensions
	COORD bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	// Setting up variables for passing to WriteConsoleOutput
	COORD characterBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD characterPosition{ 0,0 };
	SMALL_RECT consoleWriteArea = { 0,0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	//A CHAR_INFO array containing data each character
	CHAR_INFO consoleBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
	// Initialise my handles
	HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE rHnd = GetStdHandle(STD_INPUT_HANDLE);

	// Game variables
	GAME_STATE currentGameState = SPLASH;
	Background background;
	Player player;
	Explosion explosion;
};

#endif //!GAME_H

