
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"  // MessageID

unsigned int maxClients;
unsigned short serverPort;

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
};

enum
{
	ID_MESSAGE = ID_USER_PACKET_ENUM
};

#pragma pack(push, 1)
struct Message
{
	RakNet::MessageID id;
	char meme[100];
};
#pragma pack(pop)

int main(void)
{
	char str[512];
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;
	RakNet::Packet *packet;

	printf("(C) or (S)erver?\n");
	fgets(str, 512, stdin);
	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else {
		printf("Enter server port\n");
		fscanf(stdin, "%i", &serverPort);
		RakNet::SocketDescriptor sd(serverPort, 0);

		printf("Enter max clients\n");
		fscanf(stdin, "%i", &maxClients);
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}

	if (isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(maxClients);
	}
	else {
		printf("Using ip 127.0.0.1\n");
		strcpy(str, "127.0.0.1");
		printf("Starting the client.\n");
		printf("Enter server port number.\n");
		fscanf(stdin, "%i", &serverPort);
		peer->Connect(str, serverPort, 0, 0);

	}

	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");

				Message msg;
				msg.id = ID_MESSAGE;
				strcpy(msg.meme, "\nHi I'm a client, I will disconnect in 5 seconds\n");

				peer->Send((char*)&msg, sizeof(msg), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
				break;
			case ID_NEW_INCOMING_CONNECTION:
			{
				printf("A connection is incoming.\n");

				Message msg;
				msg.id = ID_MESSAGE;
				strcpy(msg.meme, "\nWelcome, I will be your server today\n");

				peer->Send((char*)&msg, sizeof(msg), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer) {
					printf("A client has disconnected.\n");
				}
				else {
					printf("We have been disconnected.\n");
				}
				break;
			case ID_CONNECTION_LOST:
				if (isServer) {
					printf("A client lost the connection.\n");
				}
				else {
					printf("Connection lost.\n");
				}
				break;
			case ID_GAME_MESSAGE_1:
			{
				RakNet::RakString rs;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				printf("%s\n", rs.C_String());
			}
			break;

			case ID_MESSAGE:
			{
				Message* msg = (Message*)packet->data;
				printf("%s", msg->meme);

				if (!isServer)
				{
					Sleep(5000);

					Message msg;
					msg.id = ID_MESSAGE;
					strcpy(msg.meme, "I'm outta here\n");

					peer->Send((char*)&msg, sizeof(msg), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

					peer->CloseConnection(packet->systemAddress, true);
				}
			}
			break;
			default:
				
				break;
			}
		}
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}
