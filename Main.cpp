// INCLUDES
#include <Windows.h>
#include <chrono>
#include "GameObjects.h"
#include "Game.h"

// TYPEDEFS
typedef std::chrono::steady_clock::time_point Time;
typedef std::chrono::high_resolution_clock HiResClock;
typedef std::chrono::duration<float> TimeDiff;

int main()
{
	Game gameinstance;

	// Initialise our console window
	gameinstance.Initialise();

	// Initialise variables
	float deltaTime = 0.0f;
	Time previousFrameTime = HiResClock::now();
	Time currentFrameTime = HiResClock::now();

	// Main game loop
	bool exitGame = false;
	while (!exitGame)
	{		
			// Calculate our delta time
			currentFrameTime = HiResClock::now();
			TimeDiff diff = currentFrameTime - previousFrameTime;
			deltaTime = diff.count();

			if (deltaTime >= (1.0f / FRAME_RATE))
			{
				// Update our application
				gameinstance.Update(deltaTime);

				// Cache the timestamp of this frame
				previousFrameTime = currentFrameTime;
			}

			gameinstance.Draw();
	}
		return 0;	
}