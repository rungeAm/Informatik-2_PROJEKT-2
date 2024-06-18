#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h> //for InetPton
#include <sstream>
#include <thread>
#include <vector>
#include "user.h"
#include "peer_functions.h"



using  std::endl;
using  std::cout;
using  std::cin;
using std::string;

bool DEBUG_MODE = 1;
int Port = 26005;

//string IP1 = "192.168.178.25";
//string IP2 = "192.168.178.27";
//string IP3 = "192.168.178.24";

//string IP1 = "192.168.1.102";
//string IP2 = "192.168.1.105";


string IP1 = "192.168.68.114";
string IP2 = "192.168.68.10";

User firstUser, joiningUser;


//==================================================

int main()
{
	WSACleanup();
	bool firstUserC = 0;

	start_(DEBUG_MODE);

	cout << "First User? " << endl;
	cin >> firstUserC;
	if (!firstUserC)
	{
	    joiningUser.User_connect_to_P2P( IP1, IP2, Port, 0, DEBUG_MODE);

		joiningUser.User_send_handshake_backconnect_friendrequest_message( 0, IP1, IP2, Port, 0, DEBUG_MODE);

		joiningUser.close_connectSocket_(0, DEBUG_MODE);

		joiningUser.User_bind_listen_accept( IP1, IP2, Port, 0, DEBUG_MODE);

		joiningUser.User_handle_handshake_backconnect_friendrequest_message( 1, IP1, IP2, Port, 0, DEBUG_MODE);

		joiningUser.send_recieve(1, 0, DEBUG_MODE)


	}
	else
	{
	

		firstUser.User_bind_listen_accept(IP2, IP1, Port, 0, DEBUG_MODE);

		firstUser.User_handle_handshake_backconnect_friendrequest_message(0, IP2, IP1, Port, 0, DEBUG_MODE);

		firstUser.close_acceptSocket_(0, DEBUG_MODE);

		//--------------------------
		firstUser.User_connect_to_P2P( IP2, IP1, Port, 0, DEBUG_MODE);

		firstUser.User_send_handshake_backconnect_friendrequest_message( 1, IP1, IP2, Port,  0, DEBUG_MODE);

		firstUser.send_recieve(0, 0, DEBUG_MODE)
	}
	
	return 0;
}


//===================================================



//===================================================