#include <iostream>
#include <string>

using  std::endl;
using  std::cout;
using  std::cin;
using std::string;


#include <iostream>


void UI_Startup()
{
	char input;

	std::cout << "    ..........................................................\n";
	std::cout << "    .                                                        .\n";
	std::cout << "    .      INFO2 PROJECT from Lajos, Matthias, Andi          .\n";
	std::cout << "    .                                                        .\n";
	std::cout << "    .      hit [j] to join an existing P2P Network!          .\n";
	std::cout << "    .      hit [i] to create a new P2P Network!              .\n";
	std::cout << "    .                                                        .\n";
	std::cout << "    ..........................................................\n\n";
	std::cout << "Eingabe: ";


	std::cin >> input;

	if (input == 'j')
	{
		//code
	}
	else if (input == 'i')
	{

		//code
	}
	else
	{
		std::cout << "Bitte nur j oder i eingeben! " << std::endl;
	
	}
}

void UI_sendMessage()
{
	std::cout << "    ..........................................................\n";
	std::cout << "    .                                                        .\n";
	std::cout << "    .      Choose an action!                                 .\n";
	std::cout << "    .                                                        .\n";
	std::cout << "    .      hit [m] to write in the chat!                     .\n";
	std::cout << "    .      hit [s] to show who's connected!                  .\n";
	std::cout << "    .      hit [l] to leave the chat!                        .\n";
	std::cout << "    .                                                        .\n";
	std::cout << "    ..........................................................\n\n";

	char input;
	std::cout << "Eingabe: ";


	std::cin >> input;

	if (input == 'm')
	{
		std::string message;
		char buffer[1024] = "";
		std::cout << "Geben Sie eine Nachricht ein: ";
		std::getline(std::cin >> std::ws, message);
	}
	else if (input == 's')
	{

		//code
	}
	else if (input == 'l')
	{
		//
	}

	else
	{
		std::cout << "Bitte nur m, s oder l eingeben! " << std::endl;
	
	}
}
