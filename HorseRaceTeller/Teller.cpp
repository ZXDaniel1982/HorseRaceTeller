#include <iostream>
#include <sstream>
#include <vector>
#include "Teller.h"

/**
 * The horse racing teller machine
 */
Teller::Teller(std::shared_ptr<IGamePlayers> aGamePlayers,
	std::shared_ptr<IGameFunds> aGameFunds,
	std::shared_ptr<ILogger> aLogger)
	: iGamePlayers(aGamePlayers),
	iGameFunds(aGameFunds),
	iKeepRunning(true),
	iLogger(aLogger)
{
	DisplayInfo();
}

/**
 * Start the teller machine, it will keep running until
 * customer input command "Q" or "q"
 */
void Teller::Run()
{
	while (iKeepRunning) {
		Listen();
	}
}

/**
 * Customer input command "Q" or "q", quit the application
 */
void Teller::Exit()
{
	iKeepRunning = false;
}

/**
 * The teller machine will continously waiting for cumstomer's input
 */
void Teller::Listen()
{
	std::string input;
	std::getline(std::cin, input);
	ExecuteCommand(input);
}

/**
 * Parse customer's input and execute the command
 */
void Teller::ExecuteCommand(std::string aInput)
{
	std::stringstream ss(aInput);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	switch (vstrings.size()) {
	case 0:
		break;
	case 1:
		if ((vstrings[0] == "R") || (vstrings[0] == "r")) {
			/**
			 * Refill all cashes inventory back to origin
			 */
			iGameFunds->Refill();
		}
		else if ((vstrings[0] == "Q") || (vstrings[0] == "q")) {
			/**
			 * Exit the teller application
			 */
			Exit();
		}
		else {
			std::stringstream output;
			output << "Invalid Command: " << aInput;
			iLogger->PrintLine(ss.str());
		}
		break;
	case 2:
		if ((vstrings[0] == "W") || (vstrings[0] == "w")) {
			/**
			 * Setup the winner horse
			 */
			iGamePlayers->SetWinner(vstrings[1]);
		}
		else {
			/**
			 * Check if a horse is a winner
			 */
			std::cout << "command " << vstrings[0] << " " << vstrings[1] << std::endl;
			iGamePlayers->CheckWinner(vstrings[0], vstrings[1]);

		}
		break;
	default:
		std::stringstream output;
		output << "Invalid Command: " << aInput;
		iLogger->PrintLine(ss.str());
		break;
	}
	DisplayInfo();
}

void Teller::DisplayInfo()
{
	iGameFunds->DisplayInfo();
	iGamePlayers->DisplayInfo();
}
