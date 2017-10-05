#include <iostream>
#include "MyThread.h"

class GameState
{
	int state;
};

int helloThreadLaunch(void* params)
{
	for (int i = 0; i < 1000; ++i)
	{
		std::cout << "Hey man ";
	}

	return 0;
}

int goodbyeThreadLaunch(void* params)
{
	for (int i = 0; i < 1000; ++i)
	{
		std::cout << "Cya dude ";
	}

	return 0;
}



int main()
{
	GameState theState;

	MyThread helloThread = { 0 }, goodbyeThread = { 0 };

	threadLaunch(&helloThread, helloThreadLaunch, &theState);
	threadLaunch(&goodbyeThread, goodbyeThreadLaunch, &theState);

	while (helloThread.flag <= 0 || goodbyeThread.flag <= 0);
}