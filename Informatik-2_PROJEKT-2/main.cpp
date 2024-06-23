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

//LajosROUTER
//string IP1 = "192.168.178.25";
//string IP2 = "192.168.178.27";
//string IP3 = "192.168.178.24";

//HOME
//string IP1 = "192.168.1.102";
//string IP2 = "192.168.1.105";

//PHONE
string IP1 = "192.168.233.114";
string IP2 = "192.168.233.10";

User firstUser, joiningUser;


//==================================================

int main()
{
	WSACleanup();
	bool firstUserC = 0;
	int fatal_err = 0;

	start_(DEBUG_MODE);

	cout << "First User? " << endl;
	cin >> firstUserC;
	if (!firstUserC)
	{
	

	  fatal_err =  joiningUser.User_connect_to_P2P( IP1, IP2, Port, 0, DEBUG_MODE);
	  if (fatal_err == -1) return 0;

	  fatal_err = joiningUser.User_send_handshake_backconnect_friendrequest_message( 0, IP1, IP2, Port, 0, DEBUG_MODE);
	  if (fatal_err == -1) return 0;

	  fatal_err = joiningUser.close_connectSocket_(0, DEBUG_MODE);
	  if (fatal_err == -1) return 0;

	  std::thread myThread([&]() {
		  int fatal_err = joiningUser.User_bind_listen_accept(IP1, IP2, Port, 0, DEBUG_MODE);
		  if (fatal_err == -1) {
			  std::cout << "Error in User_bind_listen_accept!" << std::endl;

		  }
		  });

	  myThread.join();

	  fatal_err = joiningUser.User_handle_handshake_backconnect_friendrequest_message( 1, IP1, IP2, Port, 0, DEBUG_MODE);
	  if (fatal_err == -1) return 0;

	  fatal_err = joiningUser.send_recieve(1, 0, DEBUG_MODE);
	  if (fatal_err == -1) return 0;


	}
	else
	{
		std::thread myThread([&]() {
			int fatal_err = firstUser.User_bind_listen_accept(IP2, IP1, Port, 0, DEBUG_MODE);
			if (fatal_err == -1) {
				std::cout << "Error in User_bind_listen_accept!" << std::endl;
			
			}
			});

		myThread.join();

		//fatal_err = firstUser.User_bind_listen_accept(IP2, IP1, Port, 0, DEBUG_MODE);
		//if (fatal_err == -1) return 0;

		fatal_err = firstUser.User_handle_handshake_backconnect_friendrequest_message(0, IP2, IP1, Port, 0, DEBUG_MODE);
		if (fatal_err == -1) return 0;

		fatal_err = firstUser.close_acceptSocket_(0, DEBUG_MODE);
		if (fatal_err == -1) return 0;

		//--------------------------
		fatal_err = firstUser.User_connect_to_P2P( IP2, IP1, Port, 0, DEBUG_MODE);
		if (fatal_err == -1) return 0;

		fatal_err = firstUser.User_send_handshake_backconnect_friendrequest_message( 1, IP2, IP1, Port,  0, DEBUG_MODE);
		if (fatal_err == -1) return 0;

		fatal_err = firstUser.send_recieve(0, 0, DEBUG_MODE);
	}  if (fatal_err == -1) return 0;
	
	return 0;
}


//===================================================



//===================================================