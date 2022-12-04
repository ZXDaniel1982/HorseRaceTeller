#include <map>
#include <iostream>
#include <regex>
#include <sstream>
#include "Teller.h"

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

void Teller::Run()
{
	while (iKeepRunning) {
		Listen();
	}
}

void Teller::Exit()
{
	iKeepRunning = false;
}


void Teller::Listen()
{
	std::string input;
	std::getline(std::cin, input);
	ExecuteCommand(input);
}

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
			iGameFunds->Refill();
		}
		else if ((vstrings[0] == "Q") || (vstrings[0] == "q")) {
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
			iGamePlayers->SetWinner(vstrings[1]);
		}
		else {
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
	iGamePlayers->DisplayInfo();
	iGameFunds->DisplayInfo();
}
