#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h> //for InetPton
#include <sstream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "user.h"
#include "peer_functions.h"



using  std::endl;
using  std::cout;
using  std::cin;
using std::string;

bool DEBUG_MODE = 1;
int Port = 26000;

//LajosROUTER
string IP1 = "192.168.178.25";
string IP2 = "192.168.178.27";
string IP3 = "192.168.178.24";

//HOME
//string IP1 = "192.168.1.102";
//string IP2 = "192.168.1.105";

//PHONE
//string IP1 = "192.168.233.114";
//string IP2 = "192.168.233.10";
//string IP3 = "192.168.233.232";

User firstUser, joiningUser;


//==================================================

int main()
{


	string OWNIP;
	string CONNECTIP;

	WSACleanup();
	bool firstUserC = 0;
	int fatal_err = 0;

	start_(DEBUG_MODE);
	cout << "enter own IP: ";
	cin >> OWNIP;
	if (OWNIP == "a")
	{
		OWNIP = IP1;
	}
	else if (OWNIP == "b")
	{
		OWNIP = IP2;
	}
	else if (OWNIP == "c")
	{
		OWNIP = IP3;
	}
	else
	{
		OWNIP = OWNIP;
	}



	cout << "enter connect IP!";
	cin >> CONNECTIP;
	if (CONNECTIP == "a")
	{
		CONNECTIP = IP1;
	}
	else if (CONNECTIP == "b")
	{
		CONNECTIP = IP2;
	}
	else if (CONNECTIP == "c")
	{
		CONNECTIP = IP2;
	} 
	else
	{
		CONNECTIP = CONNECTIP;
	}

	cout << "First User? " << endl;
	cin >> firstUserC;
	if (!firstUserC)
	{
	

	  fatal_err =  joiningUser.User_connect_to_P2P(OWNIP, CONNECTIP, Port, 0, DEBUG_MODE);
	  if (fatal_err == -1) return 0;

	  fatal_err = joiningUser.User_send_handshake_backconnect_friendrequest_message( 0, OWNIP, CONNECTIP, Port, 0, DEBUG_MODE);
	  if (fatal_err == -1) return 0;

	  fatal_err = joiningUser.close_connectSocket_(0, DEBUG_MODE);
	  if (fatal_err == -1) return 0;

	  std::string toConnectIP1 = joiningUser.IP_Store.back();

	  joiningUser.create_mainSocket(DEBUG_MODE);


	  std::thread myThread1([&]() {
		  cout << "entering listen Thread 1! " << endl << endl;
		  int fatal_err = joiningUser.User_bind_listen_accept(OWNIP, toConnectIP1, Port, 0, DEBUG_MODE);
		  if (fatal_err == -1) {
			  std::cout << "Error in User_bind_listen_accept!" << std::endl;

		  }
		  });
/*
	  std::string toConnectIP2 = joiningUser.IP_Store.back();
	  
	  std::thread myThread2([&]() {
		  cout << "entering listen Thread 2! " << endl << endl;
		  int fatal_err = joiningUser.User_bind_listen_accept(OWNIP, toConnectIP2, Port, 1, DEBUG_MODE);
		  if (fatal_err == -1) {
			  std::cout << "Error in User_bind_listen_accept!" << std::endl;

		  }
		  });

	  /*
	  std::thread myThread3([&]() {
		  int fatal_err = joiningUser.User_bind_listen_accept(OWNIP, toConnectIP1, Port, 2, DEBUG_MODE);
		  if (fatal_err == -1) {
			  std::cout << "Error in User_bind_listen_accept!" << std::endl;

		  }
		  });


	  std::thread myThread4([&]() {
		  int fatal_err = joiningUser.User_bind_listen_accept(OWNIP, toConnectIP1, Port, 3, DEBUG_MODE);
		  if (fatal_err == -1) {
			  std::cout << "Error in User_bind_listen_accept!" << std::endl;

		  }
		  });

*/

	  myThread1.join();
	// myThread2.join();
	//  myThread3.join();
	//  myThread4.join();

	  fatal_err = joiningUser.User_handle_handshake_backconnect_friendrequest_message( 1, OWNIP, CONNECTIP, Port, 0, DEBUG_MODE);
	  if (fatal_err == -1) return 0;

	  fatal_err = joiningUser.send_recieve(1, 0, DEBUG_MODE);
	  if (fatal_err == -1) return 0;


	}
	else
	{

		firstUser.create_mainSocket(DEBUG_MODE);

		std::thread myThread1([&]() {
			cout << "entering listen Thread 1! " << endl << endl;
			int fatal_err = firstUser.User_bind_listen_accept(OWNIP, CONNECTIP, Port, 0, DEBUG_MODE);
			if (fatal_err == -1) {
				std::cout << "Error in User_bind_listen_accept!" << std::endl;
			
			}
			});
		/*
		string secondIP = firstUser.IP_Store.back();
		
		std::thread myThread2([&]() {
			cout << "entering listen Thread 2! " << endl << endl;
			int fatal_err = firstUser.User_bind_listen_accept(OWNIP, secondIP, Port, 1, DEBUG_MODE);
			if (fatal_err == -1) {
				std::cout << "Error in User_bind_listen_accept!" << std::endl;

			}
			});
		
		std::thread myThread3([&]() {
			int fatal_err = firstUser.User_bind_listen_accept(OWNIP, CONNECTIP, Port, 2, DEBUG_MODE);
			if (fatal_err == -1) {
				std::cout << "Error in User_bind_listen_accept!" << std::endl;

			}
			});

		std::thread myThread4([&]() {
			int fatal_err = firstUser.User_bind_listen_accept(OWNIP, CONNECTIP, Port, 3, DEBUG_MODE);
			if (fatal_err == -1) {
				std::cout << "Error in User_bind_listen_accept!" << std::endl;

			}
			});

			*/
		myThread1.join();
		//myThread2.join();
		//myThread3.join();
		//myThread4.join();

		//fatal_err = firstUser.User_bind_listen_accept(IP2, IP1, Port, 0, DEBUG_MODE);
		//if (fatal_err == -1) return 0;

		fatal_err = firstUser.User_handle_handshake_backconnect_friendrequest_message(0, OWNIP, CONNECTIP, Port, 0, DEBUG_MODE);
		if (fatal_err == -1) return 0;

		fatal_err = firstUser.close_acceptSocket_(0, DEBUG_MODE);
		if (fatal_err == -1) return 0;

		//--------------------------
		fatal_err = firstUser.User_connect_to_P2P(OWNIP, CONNECTIP, Port, 0, DEBUG_MODE);
		if (fatal_err == -1) return 0;

		fatal_err = firstUser.User_send_handshake_backconnect_friendrequest_message( 1, OWNIP, CONNECTIP, Port,  0, DEBUG_MODE);
		if (fatal_err == -1) return 0;

		fatal_err = firstUser.send_recieve(0, 0, DEBUG_MODE);
	}  if (fatal_err == -1) return 0;
	
	return 0;
}


//===================================================



//===================================================