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

	int giveIP(std::vector<std::string>& IP_Save)
	{
		static int count = 0;
		string outputIP;

		if (count < IP_Store.size())
		{
			outputIP = IP_Store[count];
			count++;
			IP_Save.push_back(outputIP);
			return 0;

		}
		else if (count <= IP_Store.size())
		{
			count = 0;
			return -1;
		}



	}

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

		if (debug)cout << "created and stored acceptSocekt! " << endl;

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

	int sendSth(int connectionNr, bool debug)
	{
		
			string message =
				"In einem kleinen Dorf, so still und rein, lebte ein Junge, sein Name war Lajos, so fein.Mit leuchtenden Augen, so klar und hell, war er der Liebling, von nah und fern, so schnell.Er spielte im Wald, mit Tieren vertraut, hörte den Wind, wie er Geschichten laut.Sein Lachen klang wie ein fröhlicher Gesang, der über die Wiesen und Felder erklang.Eines Tages, so strahlend und klar, sah Lajos etwas, das wunderbar war.Ein altes Buch, verstaubt und grau, versteckt im Moos, unter einem Baum, ganz genau.Er öffnete es, seine Finger so zart, die Seiten erzählten von einer fernen Art.Von Rittern und Drachen, von Mut und von List, Lajos war gefangen, in dieser Welt, die ihn küsst.Er träumte von Abenteuern, von Ehre und Ruhm, von Burgen und Schlössern, in der Ferne, im Ruhm.Doch am Abend, wenn die Sonne sank, kehrte er heim, der Mond ihm Dank.In der Stille der Nacht, wenn Sterne glühn, erzählte er Geschichten, von Dingen, die sprühn.Seine Stimme so weich, wie ein sanfter Wind, der von Träumen erzählt, die im Herzen sind.Lajos, der Junge, so klug und weise, lebte im Jetzt, doch reiste auf leise, in Welten der Fantasie, so groß und weit, war er ein Held, zu jeder Zeit.Und so wuchs er auf, Tag für Tag, mit einem Lächeln, das jeder mag.Sein Herz voller Träume, sein Geist voller Mut, Lajos, der Junge, der alles gut tut.";
				
			char buffer[1024] = { 0 };
			bool HandshakeOK = 0;

			int sendResult = send(connectSockets[connectionNr], message.c_str(), message.length(), 0);
			if (sendResult == SOCKET_ERROR) {
				if (debug)
					cout << "Send failed with error: " << WSAGetLastError() << endl;
			}
			return 0;
	}

	int recieveSth(int connecctionNr, bool debug)
	{
		char buffer[1024] = "";
		recv(acceptSockets[connecctionNr], buffer, 1023, 0);

		cout << "Lajos sais: " << (string)buffer << endl;

		return 0;
	}

	int sendBackconnect(int connectionNr, bool debug)
	{

		std::string message = "BACKCONNECT " + ownIP;

		int sendResult = send(connectSockets[connectionNr], message.c_str(), message.length(), 0);
		if (sendResult == SOCKET_ERROR) {
			cout << "Backconnect failed with error: " << WSAGetLastError() << endl;


		}
		return 0;
	}

	int sendFriendrequest(int connectionNr, bool debug)
	{
		srand(time(NULL));

		std::string message = "FRIEND REQUEST\n\n";
	

		int err = send(connectSockets[connectionNr], message.c_str(), message.length(), 0);
		if (err == SOCKET_ERROR) 
			{
				if (debug)
				cout << "Sending Friendrequest failed with error: " << WSAGetLastError() << endl;
				return -1;
			}
		else
		{
			if (debug)cout << "Sent friend request " << endl;
			
		}

		

		char buffer[40] = { 0 };
		cout << "createdd buffer " << endl;
		err = recv(connectSockets[connectionNr], buffer, 40, 0);

		cout << "went thru recv function!" << endl;

		if (err == 0) {
			{
				if (debug)
					cout << "recieving friend IP failed with error: " << WSAGetLastError() << endl;
				return -1;
			}
			if (checkIP((string)buffer))
			{
				IP_Store.push_back(buffer);
				if (debug) cout << "recieved IP stored: " << buffer;
				return 0;
			}
			else
				cout << "IP already known!" << endl;

			return 0;
		}
	}

	int handleHandshake(int connectionNr, bool debug)
		{
			char buffer[1024] = { 0 };
			string recievedMessage = buffer;
			string handshakeSucces = "INFO2/OK\n\n";

			int recvErr = recv(acceptSockets[connectionNr], buffer, 1023, 0);
			if (recvErr == SOCKET_ERROR)
			{
				if (debug)
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
			
			int err =  recv(acceptSockets[connectionNr], buffer, 1023, 0);
			if (err == 0)
			{
				if (debug)
				cout << "backconnect failed! " << endl;
				return -1;
			}

			IP = checkBACKCONNECT((string)buffer);


			if (checkIP(IP) == 0)
			{
				IP_Store.push_back(IP);
				if (debug)
					cout << "stored IP: " << IP << endl;
				return 0;
			}
			else
				cout << "Couldnt store IP!" << endl;

		}

	int handleFriendrequest(int connectionNr, bool debug)
		{
			char buffer[1024] = "";
			//std::string IPtoSend = IP_Store[(rand() % IP_Store.size())-1];
			std::string IPtoSend = IP_Store[0];

		int err = 	recv(acceptSockets[connectionNr], buffer, 1023, 0);
		if (err == 0)
		{
			cout << "Error recieving Friendrequest! " << endl;
			return -1;
		}

			if (string(buffer) == "FRIEND REQUEST")

				if ((send(acceptSockets[connectionNr], IPtoSend.c_str(), IPtoSend.size(), 0)) == 0);
			{
				if (debug)
					cout << "Sent random IP: " << IPtoSend << endl;
				return 0;
			}
				 return -1;


			


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
