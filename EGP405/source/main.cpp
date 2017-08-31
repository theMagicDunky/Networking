
#include <stdio.h>
#include "RakNet/RakPeerInterface.h"

unsigned int maxClients;
unsigned short serverPort;

int main(void)
{
	char str[512];
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;

	printf("(C) or (S)erver?\n");
	fgets(str, 512, stdin);
	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else {
		RakNet::SocketDescriptor sd(serverPort, 0);
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}


	// TODO - Add code body here

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}
