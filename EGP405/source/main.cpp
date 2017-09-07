/*
	Duncan Carroll - 0894235
	EGP-405-02
	Lab 1 9/4/17

	Certificate of Authenticity:

	I certify that this work is entirely my own.
	The assessor of this project may reproduce this project
	and provide copies to other academic staff, and/or communicate a copy of
	this project to a plagiarism - checking service, which may retain a copy of the	project on its database.
*/

/*
	Some code taken from the RakNet manual - http://www.jenkinssoftware.com/raknet/manual/
	as per instruction of the assignment
*/

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h" 

unsigned int maxClients;
unsigned short serverPort;

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
};

//Custom enum for when a mesage is sent using the Message struct
enum
{
	ID_MESSAGE = ID_USER_PACKET_ENUM,

	// handshake exchange
	ID_USERNAME,			//client responds to connection by sending username
	ID_NEW_CLIENT_JOINED,	//server tells all clients someone joined
	ID_CLIENT_NUMBER,		//serer associates username with client number

	//message exchange
	ID_CHAT_MESSAGE,		//sent by anyone
	
	//misc
	ID_SEND_ALL				//sent by client
};



#pragma pack(push, 1)
//struct Message
//{
//	//identifier for reading what type of packet it is
//	RakNet::MessageID id;
//	//fixed string size for message to be sent
//	char message[100];
//};


struct UsernameMessage
{
	char MessageID;
	char username[31];
};

struct ClientNumberMessage
{
	char MessageID;
	unsigned int clientNumber;
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
		//automatically assigning port 127.0.0.1 per instruction of assignment
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

				////create Message struct and set packet identifier and the string to be sent
				//Message msg;
				//msg.id = ID_MESSAGE;
				//strcpy(msg.message, "\nHi I'm a client, I will disconnect in 5 seconds\n");

				////Sending Mesage struct as a packet
				//peer->Send((char*)&msg, sizeof(msg), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

				UsernameMessage username = { ID_USERNAME, "WizDaddy" };
				peer->Send((char*)&username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
				break;
			case ID_USERNAME:
			{
				// we are server, store username in dictionary
				//let everyone know who joined
				UsernameMessage *username = (UsernameMessage*)packet->data;

				username->MessageID = ID_NEW_CLIENT_JOINED;
				peer->Send((char*)&username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);

				//send new client their id
				ClientNumberMessage clientNumber = { ID_CLIENT_NUMBER, 0 };

				peer->Send((char*)&clientNumber, sizeof(clientNumber), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				break;
			}
			case ID_NEW_INCOMING_CONNECTION:
			{
				printf("A connection is incoming.\n");

				/*Message msg;
				msg.id = ID_MESSAGE;
				strcpy(msg.message, "\nWelcome, I will be your server today\n");

				peer->Send((char*)&msg, sizeof(msg), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);*/
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
			/*case ID_GAME_MESSAGE_1:
			{
				RakNet::RakString rs;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				printf("%s\n", rs.C_String());
			}
			break;*/
			//Case for receiving packet with Message struct
			//case ID_MESSAGE:
			//{
			//	Message* msg = (Message*)packet->data;
			//	printf("%s", msg->message);

			//	//when client receives message struct from server it waits 5 seconds then send goodbye message and disconnects
			//	if (!isServer)
			//	{
			//		Sleep(5000);

			//		Message msg;
			//		msg.id = ID_MESSAGE;
			//		strcpy(msg.message, "I'm outta here\n");

			//		peer->Send((char*)&msg, sizeof(msg), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

			//		peer->CloseConnection(packet->systemAddress, true);
			//	}
			//}
			break;
			default:
				
				break;
			}
		}
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}
