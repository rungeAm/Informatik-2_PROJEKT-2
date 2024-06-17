#include <iostream>
#include <string>
#include <winsock2.h>
#include "peer_functions.h"
#include "user.h"
#include <ws2tcpip.h> //for InetPton
#include <sstream>
#include <thread>


using  std::endl;
using  std::cout;
using  std::cin;
using std::string;

int start_(bool debug)
{
	WSADATA wsaData;
	int wsaErr;
	WORD wVersionRequested = MAKEWORD(2, 2);

	wsaErr = WSAStartup(wVersionRequested, &wsaData);

	if (debug)

	{
		if (wsaErr != 0)
		{
			cout << "wsaData connect error: " << wsaErr << endl;

			return -1;
		}
		else
		{
			(cout << "ws2_32.dll connected successfully!" << endl);
			return 0;
		}
	}
	return 0;

}

sockaddr_in createSockaddr(string IP, int port)
{

	sockaddr_in sAddr;
	sAddr.sin_family = AF_INET;
	sAddr.sin_port = htons(port);
	inet_pton(AF_INET, IP.c_str(), &sAddr.sin_addr.s_addr);


	return sAddr;
}
