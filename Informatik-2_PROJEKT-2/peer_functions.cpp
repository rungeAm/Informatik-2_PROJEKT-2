#include <iostream>
#include <string>
#include <winsock2.h>
#include "peer_functions.h"
#include "user.h"
#include <ws2tcpip.h> //for InetPton
#include <sstream>
#include <thread>
//#include <any>


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


string floatToString(float num)
{
	std::stringstream ss;
	string output;
	ss << num;
	ss >> output;

	return output;
}

float stringToFloat(string str)
{
	std::stringstream ss;
	float output;

	ss << str;
	ss >> output;

	return output;
}

bool check_INFO2CONNECT(std::string text, float version)
{
	string check1 = text.substr(0, 14);
	string check2 = text.substr(14, (text.size() - 1));



	if ((strcmp(check1.c_str(), "INFO2 CONNECT/")))
	{
		if ((stringToFloat(check2)) >= version)
			return true;
	}
}

bool check_INFO2OK(std::string text)
{
	if (text == "INFO2/OK\n\n")
	{
		cout << "Handshake success!" << endl;
		return true;
	}
	else
	{
		cout << "Handshake fail!" << endl;
		return false;
	}
}

std::string checkBACKCONNECT(std::string text)
{
	string check1 = text.substr(0, 11);
	string check2 = text.substr(12, (text.size() - 1));



	if ((strcmp(check1.c_str(), "BACKCONNECT ")))
	{
		return check2;
	}
	else
		cout << "backconnect error!" << endl;
}

int createMessageID()
{
	srand(time(NULL));

	return (rand() % 99999);
}

std::string get_message(std::string input)
{
	
	return input.substr(6, 1023);

}

int get_ID(std::string input)
		
{
	return ((int)stringToFloat(input.substr(0, 5)));
}

std::string cutEmptySpaces(std::string input)
{
	int nullo = input.find('\0');


	return input.substr(0, nullo);
}
		


