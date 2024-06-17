#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h> //for InetPton
#include <sstream>
#include <thread>
#include <vector>
#include "user.h"
#include "user.h"
#include "peer_functions.h"


using  std::endl;
using  std::cout;
using  std::cin;
using std::string;

bool DEBUG_MODE = 1;
int PORT = 26001;

string IP1 = "192.168.178.25";
string IP2 = "192.168.178.27";


//==================================================

int main()
{
	
	bool firstUser = 0;

	start_(DEBUG_MODE);

	cout << "First User? " << endl;
	cin >> firstUser;
	if (!firstUser)
	{
		User joiningUser;

		joiningUser.connectIP = IP1;
		joiningUser.ownIP = IP2;
		joiningUser.port = PORT;
		

		joiningUser.create_connectSocket(DEBUG_MODE);
		joiningUser.connect_(0, DEBUG_MODE);

		joiningUser.sendHandshake(0, DEBUG_MODE);

	}
	else
	{
		User firstUser;

		firstUser.ownIP = IP1;
		firstUser.port = PORT;

		firstUser.create_mainSocket(DEBUG_MODE);

		firstUser.bind_(DEBUG_MODE);
		firstUser.listen_(20, DEBUG_MODE);
		firstUser.accept_(0, DEBUG_MODE);

		firstUser.handleHandshake(0, DEBUG_MODE);
		 

	}
	
	return 0;
}


//===================================================



//===================================================