#pragma once

#define PORT 26005;


#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h> //for InetPton
#include <sstream>
#include <thread>
#include <vector>


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
	std::vector<int>ID_Store = { 000000 };
	std::vector<std::string>IP_Store = { ownIP };

	int giveIP(std::vector<std::string>& IP_Save)
	{
		static int count = 0;
		string outputIP;

		if (count < IP_Store.size())
		{
			outputIP = IP_Store[count];
			count++;
			outputIP = *IP_Store.end();
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
			connectSockets.push_back(connectSocket);

			if (connectSocket != connectSockets.back())
			{
				if (debug) cout << "Error storing connectSocket! " << endl;
				return -1;
			}
			else
				if (debug) cout << "stored connectSocket! " << endl;

			return 0;
		}



	}

	int close_connectSocket_(int connectionNr, bool debug)
	{
		closesocket(connectSockets[connectionNr]);
		return 0;
	}

	int create_mainSocket(bool debug)
	{

		mainSocket = INVALID_SOCKET;
		mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


		if (mainSocket == INVALID_SOCKET)
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

	int close_acceptSocket_(int connectionNr, bool debug)
	{
		closesocket(acceptSockets[connectionNr]);
		return 0;
	}

	int bind_(std::string IP, bool debug)
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
		if (listen(mainSocket, max_connected) == SOCKET_ERROR)			//serverSocket , Amt of max connected servants
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
		SOCKET aSock = accept(mainSocket, NULL, NULL);

		if (aSock == INVALID_SOCKET)
		{
			if (debug)
				cout << "Accept failed, Error: " << WSAGetLastError() << endl;
			closesocket(mainSocket);
			WSACleanup();
			return -1;
		}


		acceptSockets.push_back(aSock);

		if (aSock == acceptSockets.back())
		{

			if (debug)cout << "created and stored acceptSocekt! " << endl;
			else
			{
				if (debug) cout << "Error storing acceptsocket!" << endl;
					return -1;
			}
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
		if (debug)
			cout << endl << "----------enter connect_" << endl;
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
		if (debug)
			cout << endl << "----------enter sendHandshake" << endl;

		string message = "INFO2 CONNECT/" + floatToString(version);
		char buffer[1024] = { 0 };
		bool HandshakeOK = 0;

		int sendResult = send(connectSockets[connectionNr], message.c_str(), message.length(), 0);
		if (sendResult == SOCKET_ERROR) {
			if (debug)
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
		if (debug)
			cout << endl << "----------enter sendBackconnect" << endl;
		std::string message = "BACKCONNECT " + ownIP;

		int err = send(connectSockets[connectionNr], message.c_str(), message.length(), 0);
		if (err == 0) {
			cout << "Backconnect failed with error: " << WSAGetLastError() << endl;


		}
		return 0;
	}

	int sendFriendrequest(int connectionNr, bool debug)
	{
		if (debug)
			cout << endl << "----------enter sendFriendrequest" << endl;
		srand(time(NULL));

		std::string message = "FRIEND REQUEST";

		int err = 0;
		err = send(connectSockets[connectionNr], message.c_str(), message.length(), 0);
		if (err == 0)
		{
			if (debug)
				cout << "Sending Friendrequest failed with error: " << WSAGetLastError() << endl;
			return -1;
		}
		else
		{
			if (debug)cout << "Sent friend request to " << connectIP << endl;

		}



		char buffer[40];
		memset(buffer, '\0', sizeof(buffer));

		cout << "created buffer " << endl;

		err = recv(connectSockets[connectionNr], buffer, sizeof(buffer), 0);

		cout << "went thru recv function!" << endl;

		if (err == 0)
		{
			if (debug)
				cout << "recieving friend IP failed with error: " << WSAGetLastError() << endl;
			return -1;
		}
		else
		{
			std::memset(buffer, '\0', sizeof(buffer));


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
		if (debug)
			cout << endl << "----------enter handleHandshake" << endl;

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

		if (debug)
			cout << endl << "----------enter handleBackconnect" << endl;
		char buffer[1024] = { 0 };
		std::string IP;

		int err = recv(acceptSockets[connectionNr], buffer, 1023, 0);
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

		if (debug)
			cout << endl << "----------enter handleFriendrequest" << endl;

		char buffer[1024];
		memset(buffer, '\0', sizeof(buffer));

		//std::string IPtoSend = IP_Store[0];
		std::string IPtoSend = "111111";

		if (debug) cout << "IP set!" << endl;


		int err = recv(acceptSockets[connectionNr], buffer, sizeof(buffer), 0);
		if (err == 0)
		{
			cout << "Error recieving Friendrequest! " << endl;
			return -1;
		}

		if (string(buffer) == "FRIEND REQUEST")

		{
			int err2 = -1;

			err2 = send(acceptSockets[connectionNr], IPtoSend.c_str(), IPtoSend.size(), 0);

			if (err2 == 0)
			{
				if (debug)
					cout << "Sent IP: " << IPtoSend << endl;
				return 0;
			}
			else if (err2 != 0)
			{
				cout << "sending IP failed! Error: " << WSAGetLastError() << endl;
				return -1;
			}
			else
			{
				cout << "send is neigher 0 or not 0 lol " << WSAGetLastError() << endl;

				return -1;
			}
		}
			
		else
		{
			cout << "didnt recieve FRIEND REQUEST" << endl;
			return -1;
		}





	}

	int send_recieve(bool send_first, int connectionNr, bool debug)
	{
		if (debug)
			cout << endl << "----------enter send_recieve" << endl;
		bool logoff = 0;
		string input = "";
		string message = "";
		char buffer[1024] = { 0 };
		int err = 0;

		while (!logoff)
		{
			message.clear();

			if (send_first)
			{

			Repeat:
				int ID = createMessageID();

				if (checkID(ID) == -1) goto Repeat;
				this->ID_Store.push_back(ID);

				if (ID != (*ID_Store.end()))
				{
					if (debug) cout << "Error stroring ID!";
					return -1;
				}
				
				cout << "Enter message: ";

				std::getline(std::cin >> std::ws, input);

				message.append(floatToString((float)ID));
				message.append(" ");
				message.append(input);

				err = send(acceptSockets[connectionNr], message.c_str(), message.size(), 0);

				if (err != 0)
				{
					{
						cout << "Error sending message! " << endl;
					}
				}
				else
				{ }

				if (message == "/disconnect")
				{
					cout << ownIP << " disconnected! ";
					return 0;

				}
			}
			send_first = 1;

			err = recv(acceptSockets[connectionNr], buffer, 1023, 0);

			if (err != 0)
			{

				cout << "Error sending message! " << endl;

			}

			if (!(checkID(get_ID((std::string)buffer))))
			{
				err = send(acceptSockets[connectionNr], (get_message((string)buffer)).c_str(), 1023, 0);
			}

			cout << get_message((string)buffer) << endl;




		}


	}

	int checkIP(std::string IP)
	{
		
			cout << endl << "			......enter send_recieve" << endl;
		cout << "checking IP! " << endl;
		for (int i = 0; i < IP_Store.size(); i++)
		{
			if (IP == IP_Store[i]) return -1;

		}
		return 0;
	}

	int checkID(int ID)
	{
	
			cout << endl << "			......enter send_recieve" << endl;
		

		for (int i = 0; i < (ID_Store.size() - 1); i++)
		{
			if (ID == ID_Store[i])
				return -1;
		}
		return 0;

	}

	int User_connect_to_P2P(std::string ownIP, std::string connectIP, int port, int connectionNr, bool DEBUG_MODE)
	{


		this->connectIP = connectIP;
		this->ownIP = ownIP;
		this->port = port;


		create_connectSocket(DEBUG_MODE);
		connect_(connectIP, connectionNr, DEBUG_MODE);





		return 0;

	}

	int User_bind_listen_accept(std::string ownIP, std::string connectIP, int port, int connectionNr, bool DEBUG_MODE)
	{
		this->ownIP = ownIP;
		this->connectIP = connectIP;
		this->port = port;

		create_mainSocket(DEBUG_MODE);

		bind_(ownIP, DEBUG_MODE);
		listen_(20, DEBUG_MODE);
		accept_(connectionNr, DEBUG_MODE);


		return 0;


	}

	int User_handle_handshake_backconnect_friendrequest_message(bool FR, std::string ownIP, std::string connectIP, int port, int connectionNr, bool DEBUG_MODE)
	{
		int fatal_err = 0;

		fatal_err = handleHandshake(connectionNr, DEBUG_MODE);
		if (fatal_err == -1)
		{
			cout << "-1 at handleHandshake" << endl;
			return -1;
		}
		fatal_err = handleBackconnect(connectionNr, DEBUG_MODE);

		if (fatal_err == -1)
		{
			cout << "-1 at handleBackconnect" << endl;
			return -1;
		}

		if (FR)
		{
			fatal_err = handleFriendrequest(connectionNr, DEBUG_MODE);
			
		if (fatal_err == -1)
		{
			cout << "-1 at handleFriendrequest" << endl;
			return -1;
		}

		}
	



		//send&recieve

		return 0;

	}

	int User_send_handshake_backconnect_friendrequest_message(bool FR, std::string ownIP, std::string connectIP, int port, int connectionNr, bool DEBUG_MODE)
	{
		int fatal_err = 0;

		sendHandshake(connectionNr, DEBUG_MODE);
			if (fatal_err == -1)
			{
				cout << "-1 at sendHandshake" << endl;
				return -1;
			}
		sendBackconnect(connectionNr, DEBUG_MODE);
		if (fatal_err == -1)
		{
			cout << "-1 at sendBackconnect" << endl;
			return -1;
		}

		if (FR)
		{
			sendFriendrequest(connectionNr, DEBUG_MODE);
			if (fatal_err == -1)
			{
				cout << "-1 at sendFriendrequest" << endl;
				return -1;
			}
		}
	



		return 0;

	}

};


