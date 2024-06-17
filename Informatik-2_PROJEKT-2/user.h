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


	std::vector<SOCKET> connectSockets = {};

	SOCKET mainSocket = INVALID_SOCKET;
	std::vector<SOCKET> acceptSockets = {};
	sockaddr_in sAddr;

	int port;
	float version;
	std::string connectIP;
	std::string ownIP;
	std::vector<int>ID_Store;
	std::vector<std::string>IP_Store;



	int create_connectSocket(bool debug)
	{
		SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	


		if (connectSocket == INVALID_SOCKET)
		{
			if (debug)
				std::cout << "Error creating TCP-socket! " << endl;
			return -1;
		}

		else

		{
			if (debug)
				cout << "Created TCP-socket successfully!" << endl;
			this->connectSockets.push_back(connectSocket);

			return 0;
		}



	}

	int create_mainSocket(bool debug)
	{

		this->mainSocket = INVALID_SOCKET;
		this->mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


		if (this->mainSocket == INVALID_SOCKET)
		{
			if (debug)
				std::cout << "Error creating TCP-socket! " << endl;
			return -1;
		}

		else

		{
			if (debug)
				cout << "Created TCP-socket successfully!" << endl;
	

			return 0;
		}


	}

	int bind_(std::string IP,  bool debug)
	{
		sockaddr_in sAddr;
		sAddr.sin_family = AF_INET;
		sAddr.sin_port = htons(port);
		inet_pton(AF_INET, IP.c_str(), &sAddr.sin_addr.s_addr);

		if ((bind(mainSocket, (SOCKADDR*)&sAddr, sizeof(sAddr)) == SOCKET_ERROR))
		{
			if (debug)
				cout << "Binding server socket failed. Error: " << WSAGetLastError() << endl;

			closesocket(mainSocket);
			WSACleanup();
			return -1;
		}
		else
		{
			if (debug)
				cout << "Socket successfully bound to IP: " << ownIP << " and Port: " << port << endl;
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
		acceptSockets.push_back(accept(mainSocket, NULL, NULL)) ;

		if (acceptSockets[connectionNr] == INVALID_SOCKET)
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

	int connect_(std::string IP, int connectionNr, bool debug)
	{
		sockaddr_in sAddr;
		sAddr.sin_family = AF_INET;
		sAddr.sin_port = htons(port);
		inet_pton(AF_INET, IP.c_str(), &sAddr.sin_addr.s_addr);

		if ((connect(connectSockets[connectionNr], (SOCKADDR*)&sAddr, sizeof(sAddr))) != 0)
		{
			if (debug)
				cout << "connect failed. Error: " << WSAGetLastError() << endl;
			closesocket(connectSockets[connectionNr]);
			WSACleanup();
			return 0;
		}
		else
		{
			cout << "connection successfull!" << endl;
			return -1;
		}
	}

	int sendHandshake(int connectionNr, bool debug)
	{
		string message = "INFO2 CONNECT/" + floatToString(version);
		char buffer[1024] = { 0 };
		bool HandshakeOK = 0;

		int sendResult = send(connectSockets[connectionNr], message.c_str(), message.length(), 0);
		if (sendResult == SOCKET_ERROR) {
			if(debug)
			cout << "Send failed with error: " << WSAGetLastError() << endl;
		}


		//...................................................RECV INFO2OK
		int recvErr = recv(connectSockets[connectionNr], buffer, 1023, 0);
		if (recvErr == SOCKET_ERROR) cout << "recieve error: " << WSAGetLastError();
		else
		{
			check_INFO2OK((string)buffer);
			HandshakeOK = true;
		}
		return 0;
	}

	int sendBackconnect(int connectionNr, bool debug)
	{

		std::string message = "BACKCONNECT " + ownIP;

		int sendResult = send(connectSockets[connectionNr], message.c_str(), message.length(), 0);
		if (sendResult == SOCKET_ERROR) {
			cout << "Send failed with error: " << WSAGetLastError() << endl;


		}
		return 0;
	}


	int handleHandshake(int connectionNr, bool debug)
	{
		char buffer[1024] = { 0 };
		string recievedMessage = buffer;
		string handshakeSucces = "INFO2/OK\n\n";

		int recvErr = recv(acceptSockets[connectionNr], buffer, 1023, 0);
		if (recvErr == SOCKET_ERROR) 
		{
			if(debug)
			cout << "recieve error: " << WSAGetLastError();
			closesocket(acceptSockets[connectionNr]);
			WSACleanup();
		}
		else
		{
			if (debug)
				cout << "Message sent: " << buffer << endl;
		}


		if (check_INFO2CONNECT(buffer, 0.7))
		{
			cout << "Handshake begin!" << endl;
			int sendResult = send(acceptSockets[connectionNr], handshakeSucces.c_str(), handshakeSucces.length(), 0);
			return 0;
		}
		else
		{
			cout << "Handshake fail!" << endl;
			return 1;
		}

	}

	int handleBackconnect(int connectionNr, bool debug)
	{
		char buffer[1024] = "";
		std::string IP;
		recv(acceptSockets[connectionNr], buffer, 1023, 0);
		IP = checkBACKCONNECT((string)buffer);

		
		if (checkIP(IP) == 0)
		{
			IP_Store.push_back(IP);
			return 0;
		}
		else
			cout << "Couldnt store IP!" << endl;
	
	}


	int checkIP(std::string IP)
	{
		for (int i = 0; i < IP_Store.size(); i++)
		{
			if (IP == IP_Store[i]) return -1;
			
		}
		return 0;
	}


};
