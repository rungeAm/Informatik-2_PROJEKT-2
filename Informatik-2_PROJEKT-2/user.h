#pragma once

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h> //for InetPton
#include <sstream>
#include <thread>
#include <vector>

#include "user.h"
#include "peer_functions.h"


#pragma comment(lib, "Ws2_32.lib")


using  std::endl;
using  std::cout;
using  std::cin;
using std::string;


class User
{
public:


	std::vector<SOCKET> connectSockets;

	SOCKET mainSocket = INVALID_SOCKET;
	std::vector<SOCKET> acceptSockets;
	sockaddr_in sAddr;

	int port;
	std::string connectIP;
	std::string ownIP;
	std::vector<int>ID_Store;
	std::vector<std::string>IP_Store;

	int create_connectSocket(bool debug)
	{
		SOCKET* connectSocket = new SOCKET;
		*connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


		if (*connectSocket == INVALID_SOCKET)
		{
			if (debug)
				std::cout << "Error creating TCP-socket! " << endl;
			return -1;
		}

		else

		{
			if (debug)
				cout << "Created TCP-socket successfully!" << endl;
			this->connectSockets.push_back(*connectSocket);

			return 0;
		}



	}

	int create_mainSocket(bool debug)
	{

		SOCKET* acceptSocket = new SOCKET;
		*acceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


		if (*acceptSocket == INVALID_SOCKET)
		{
			if (debug)
				std::cout << "Error creating TCP-socket! " << endl;
			return -1;
		}

		else

		{
			if (debug)
				cout << "Created TCP-socket successfully!" << endl;
			this->acceptSockets.push_back(*acceptSocket);

			return 0;
		}


	}

	int bind_(int port, bool debug)
	{
		sockaddr_in sAddr;
		sAddr.sin_family = AF_INET;
		sAddr.sin_port = htons(this->port);
		inet_pton(AF_INET, this->ownIP.c_str(), &sAddr.sin_addr.s_addr);

		if ((bind(this->mainSocket, (SOCKADDR*)&sAddr, sizeof(sAddr)) == SOCKET_ERROR))
		{
			if (debug)
				cout << "Binding server socket failed. Error: " << WSAGetLastError() << endl;

			closesocket(this->mainSocket);
			WSACleanup();
			return -1;
		}
		else
		{
			if (debug)
				cout << "Socket successfully bound to IP: " << this->ownIP << " and Port: " << port << endl;
			return 0;
		}
	}

	int listen_(int max_connected, bool debug)
	{
		if (listen(this->mainSocket, max_connected) == SOCKET_ERROR)			//serverSocket , Amt of max connected servants
		{
			if (debug)
				cout << "Listen failed. Error: " << WSAGetLastError() << endl;
			closesocket(this->mainSocket);
			WSACleanup();
			return -1;
		}
		else
		{
			if (debug)
				cout << "Listening on port.." << endl;
			return 0;
		}
	}

	int accept_(int connectionNr, bool debug)
	{
		this->acceptSockets[connectionNr] = accept(this->mainSocket, NULL, NULL);

		if (this->acceptSockets[connectionNr] == INVALID_SOCKET)
		{
			if (debug)
				cout << "Accept failed, Error: " << WSAGetLastError() << endl;
			closesocket(mainSocket);
			WSACleanup();
			return -1;
		}
		else
		{
			if (debug)
				cout << "Accept successful!" << endl;
			return 0;
		}
	}

	int connect_(int connectionNr, bool debug)
	{
		sockaddr_in sAddr;
		sAddr.sin_family = AF_INET;
		sAddr.sin_port = htons(this->port);
		inet_pton(AF_INET, this->connectIP.c_str(), &sAddr.sin_addr.s_addr);

		if ((connect(this->connectSockets[connectionNr], (SOCKADDR*)&sAddr, sizeof(&sAddr))) != 0)
		{
			if (debug)
				cout << "connect failed. Error: " << WSAGetLastError() << endl;
			closesocket(this->connectSockets[connectionNr]);
			WSACleanup();
			return 0;
		}
		else
		{
			cout << "connection successfull!" << endl;
			return -1;
		}
	}


};
