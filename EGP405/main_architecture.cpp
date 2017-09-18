// INTRO TO NETWORKED ARCHITECTURE

#include <Windows.h>
#include "LobbyState.h"
#include "GameLocalState.h"

struct KeyboardState
{
	unsigned char keyboard[256];
};

void updateInput(KeyboardState* keyState)
{
	// get mouse keyboard controler state
	// save it for later

	GetKeyboardState(keyState->keyboard);
}

void updateNetworking()
{
	// receive packets
	// process all available packets and track data for later
}

void updateState(ApplicationState* state)
{
	// account for all previous updates
	// change application with respect to time and other updates

	// update chat room stream
}

void render(const ApplicationState* state)
{
	// get em
	// display chat stream
}


int main()
{
	ApplicationState* appState;
	LobbyState lobby[1];
	GameLocalState gameState[1];

	appState = lobby;

	/*appState->running = 1;

	while (appState->running)
	{
		updateInput(appState->keyState);

		updateNetworking();

		updateState(appState);

		render(appState);
	}*/

	return 0;
}