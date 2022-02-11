// This classes header
#include "Game.h"

// Includes
#include "Utility.h"
#include "Constants.h"
#include <ctime>

// Define my cunstructor
Game::Game()
{
	
}

void Game::Initialise()
{
	// Set the console title
	SetConsoleTitle(L"Lunar Lander ASCII Game Engine");

	// Set the screen's buffer size
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	//Set the window size
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
}

void Game::Update(float deltaTime)
{
	static bool exitGame = false;
	switch (currentGameState)
	{
	case SPLASH:{
		// Create a new splash screen
		static Splash splash;

		// Calculate current splash duration
		splash.duration += deltaTime;

		if (splash.duration < 3.0f)
		{
			// Draw our Splash Image
			WriteImageToBuffer(consoleBuffer, splash.CHARACTERS, splash.COLOURS, splash.HEIGHT, splash.WIDTH,
				(SCREEN_WIDTH / 2) - (splash.WIDTH / 2), (SCREEN_HEIGHT / 2) - (splash.HEIGHT / 2));
		}
		else
		{
			// Move to menu state
			splash.duration = 0.0f;
			currentGameState = MENU;
		}

		break;

	}
	case MENU:	{
		// Define begin time and score
		time(&player.oldTime);
				
		// Clear any previous images
		ClearScreen(consoleBuffer);

		// Print out our text options
		WriteTextToBuffer(consoleBuffer, "1. PLAY: ", ((SCREEN_WIDTH / 2) - 7), ((SCREEN_HEIGHT / 2) - 4));
		WriteTextToBuffer(consoleBuffer, "2. SCOREBOARD: ", ((SCREEN_WIDTH / 2) - 7), ((SCREEN_HEIGHT / 2) - 2));
		WriteTextToBuffer(consoleBuffer, "3. QUIT: ", ((SCREEN_WIDTH / 2) - 7), (SCREEN_HEIGHT / 2));

		if (GetAsyncKeyState(KEY_1))
		{
			currentGameState = PLAY;
		}
		if (GetAsyncKeyState(KEY_2))
		{
			currentGameState = SCOREBOARD;
		}
		if (GetAsyncKeyState(KEY_3))
		{
			exitGame = true;
		}

		break;
	}
	case PLAY:	{	
		time(&player.newTime);
				
		if (GetAsyncKeyState(KEY_ESC))
		{
			exitGame = true;
		}

		if (GetAsyncKeyState(KEY_ENTER))
		{
			player.Reset();
			currentGameState = MENU;
		}

		if (!player.hasLanded && !player.hasCrashed)
		{
			time(&player.newTime);
			player.newTime -= player.oldTime;
						
			// Take the input
			if (GetAsyncKeyState(KEY_ESC))
			{
				// Set our exit game flag
				exitGame = true;
			}
			if (GetAsyncKeyState(KEY_W) && player.fuel > 0.0f)
			{
				player.isAccelerating = true;
				// Expend fuel
				player.fuel -= FUEL_CONSUMPTION_RATE;
			}
			if (GetAsyncKeyState(KEY_A))
			{
				--player.XPos;
			}
			if (GetAsyncKeyState(KEY_D))
			{
				++player.XPos;
			}

			// Should we accelerate?
			if (player.isAccelerating)
			{
				player.acceleration += (ACCELERATION_RATE * deltaTime);
			}
			else
			{
				player.acceleration -= (DECELERATION_RATE * deltaTime);
			}

			// Reset acceleration flag every frame
			player.isAccelerating = false;

			// Clamp our acceleration
			player.acceleration = ClampFloat(player.acceleration, 0.0f, 1.5f);

			if (player.acceleration >= 1.0f)
			{
				player.YPos--;
			}
			else if (player.acceleration < 0.5f)
			{
				player.YPos++;
			}

			// Clamp the input
			player.XPos = ClampInt(player.XPos, 0, (SCREEN_WIDTH - player.WIDTH));
			player.YPos = ClampInt(player.YPos, 0, (SCREEN_HEIGHT - player.HEIGHT));

			// Get the two characters under our landing gear
			char bottomLeftChar = background.CHARACTERS[player.XPos + SCREEN_WIDTH * (player.YPos + (player.HEIGHT - 1))];
			char bottomRightChar = background.CHARACTERS[(player.XPos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.YPos + (player.HEIGHT - 1))];

			// Did we land or crash?
			// Landed?
			if (bottomLeftChar == '_' && bottomRightChar == '_')
			{
				player.hasLanded = true;
				// LandingScore
				if (player.XPos <= 13 && player.XPos >= 10 && player.YPos == 26)
				{
				player.landingScore = 1000;
				}
				else if (player.XPos <= 28 && player.XPos >= 24 && player.YPos == 12)
				{
					player.landingScore = 650;
				}
				else if (player.XPos <= 31 && player.XPos >= 29 && player.YPos == 9)
				{
					player.landingScore = 500;
				}
				else if (player.XPos <= 36 && player.XPos >= 33 && player.YPos == 12)
				{
					player.landingScore = 400;
				}
				else if (player.XPos <= 52 && player.XPos >= 48 && player.YPos == 26)
				{
					player.landingScore = 300;
				}
				else if (player.XPos <= 72 && player.XPos >= 68 && player.YPos == 27)
				{
					player.landingScore = 200;
				}
				else if (player.XPos <= 100 && player.XPos >= 96 && player.YPos == 18)
				{
					player.landingScore = 250;
				}
				else if (player.XPos <= 138 && player.XPos >= 134 && player.YPos == 19)
				{
					player.landingScore = 350;
				}			
			}
			// Background objects (sun, cloud)
			else if (bottomLeftChar == 'O' && bottomRightChar != 'O')
			{
				player.hasCrashed = false;
				player.hasLanded = false;
			}
			// Crashed?
			else if (bottomLeftChar != ' ' || bottomRightChar != ' ')
			{
				player.hasCrashed = true;
				// CrashingScore
				player.crashingScore = 0;
			}
		}

			// Clear the previous 'frame' before we start building the next one
			ClearScreen(consoleBuffer);

			// Draw background image
			WriteImageToBuffer(consoleBuffer, background.CHARACTERS, background.COLOURS, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);

			// Draw player Crashing
			if (player.hasCrashed)
			{
				static float explosionFlashTimer = 0.0f;

				explosionFlashTimer += deltaTime;

				if (explosionFlashTimer >= 0.5f)
				{
					// Draw our explosion
					WriteImageToBuffer(consoleBuffer, explosion.CHARACTERS, explosion.COLOURS, explosion.HEIGHT, explosion.WIDTH, player.XPos, player.YPos);
					explosionFlashTimer = 0.0f;
				}
				else
				{
					// Draw empty characters
					WriteImageToBuffer(consoleBuffer, explosion.EMPTY_CHARACTERS, explosion.COLOURS, explosion.HEIGHT, explosion.WIDTH, player.XPos, player.YPos);
				}

				WriteTextToBuffer(consoleBuffer, "YOU MADE A DISASTER, WE CRASHED!!!", (SCREEN_WIDTH / 2) + 20, (SCREEN_HEIGHT / 2) - 10);
				WriteTextToBuffer(consoleBuffer, "Press 'Enter' to return to the menu", (SCREEN_WIDTH / 2) + 20, (SCREEN_HEIGHT / 2) -8);
			}
			else if (player.hasLanded)
			{
				WriteTextToBuffer(consoleBuffer, "YOU MANAGED TO LAND SAFETELY!", (SCREEN_WIDTH / 2) + 20, (SCREEN_HEIGHT / 2) - 10);
				WriteTextToBuffer(consoleBuffer, "Press 'Enter' to return to the menu", (SCREEN_WIDTH / 2) + 20, (SCREEN_HEIGHT / 2) - 8);
			}
			else
			{
				// Draw player image
				WriteImageToBuffer(consoleBuffer, player.CHARACTERS, player.COLOURS, player.HEIGHT, player.WIDTH, player.XPos, player.YPos);
			}

			// Draw UI Text
			player.score = player.newTime * 2.5;
						
			if (!player.hasLanded && !player.hasCrashed)
			{
				// Top left hand side
				WriteTextToBuffer(consoleBuffer, "TIME: " + std::to_string(player.newTime), 1, 1);
				WriteTextToBuffer(consoleBuffer, "FLYING SCORE: " + std::to_string(player.score), 1, 0);
				WriteTextToBuffer(consoleBuffer, "FUEL: " + std::to_string(player.fuel), 1, 2);		

				// Top right hand side
				WriteTextToBuffer(consoleBuffer, "ALTITUDE: " + std::to_string(40 - player.YPos), 135, 3);
				WriteTextToBuffer(consoleBuffer, "LONGITUDE: " + std::to_string(player.XPos), 135, 4);
			}
			else
			{
				if (player.hasLanded && !player.hasCrashed)
				{
					
					WriteTextToBuffer(consoleBuffer, "TIME: to toast", 1, 1);
					WriteTextToBuffer(consoleBuffer, "CONGRATULATIONS!! YOUR FINAL SCORE IS " + std::to_string(player.landingScore), 1, 0);
				}
				else if (!player.hasLanded && player.hasCrashed)
				{
					WriteTextToBuffer(consoleBuffer, "TIME: to cry", 1, 1);
					WriteTextToBuffer(consoleBuffer, "I DON'T THINK YOU DESERVE A SCORE...", 1, 0);
				}
			}
		break;
	}
	case SCOREBOARD: {
		// Clear any previous images
		ClearScreen(consoleBuffer);

		// Draw background image
		WriteImageToBuffer(consoleBuffer, background.CHARACTERS, background.COLOURS, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);

		// Print out our text options
		int finalScore = 0;
		finalScore = player.crashingScore + player.landingScore;

		if (finalScore != 0)
		{
			WriteTextToBuffer(consoleBuffer, "Congratulations!! Your last score has been: " + std::to_string(finalScore), 90, 5);
			WriteTextToBuffer(consoleBuffer, "Press 'Enter' to return to the menu", 90, 7);
		}
		else
		{
			WriteTextToBuffer(consoleBuffer, "You crashed, you must keep playing to get a better score :(", 90, 5);
			WriteTextToBuffer(consoleBuffer, "Press 'Enter' to return to the menu", 90, 7);
		}
		if (GetAsyncKeyState(KEY_ENTER))
		{
			player.Reset();
			currentGameState = MENU;
		}
		break;
	}
	}
}

void Game::Draw()
{
	//Kick the draw
	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
}