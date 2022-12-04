#include "pch.h"
#include "CppUnitTest.h"

#include "../HorseRaceTeller/Teller.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Simulate horses play the race game, a replacement for config XML file
std::map<int, std::pair<std::string, int>> horseTestCases = {
	{1, {"That Darn Gray Cat", 5} },
	{2, {"Fort Utopia", 10}},
	{3, {"Count Sheep", 9}},
	{4, {"Ms Traitour", 4}},
	{5, {"Real Princess", 3}},
	{6, {"Pa Kettle", 5}},
	{7, {"Gin Stinger", 6 }}
};

// Simulate all funds stored int the teller machine, a replacement for config XML file
std::map<int, std::pair<int, int>> cashTestCases = {
	{1, {10, 10}},
	{5, {10, 10}},
	{10, {10, 10}},
	{20, {10, 10}},
	{100, {10, 10}}
};

// Simulate customer's input, followed by expected outcomes
std::map<int, std::pair<std::string, std::string>> tellerTestCases = {
	{0, {"",
		"Inventory:$1,10$5,10$10,10$20,10$100,10Horses:1,That Darn Gray Cat,5,won2,Fort Utopia,10,lost3,"\
		"Count Sheep,9,lost4,Ms Traitour,4,lost5,Real Princess,3,lost6,Pa Kettle,5,lost7,Gin Stinger,6,lost"}},
	{1, {"1 55",
		"Payout: That Darn Gray Cat,275Dispensing: 275$1,0$5,1$10,1$20,3$100,2Inventory:$1,10$5,9$10,9$20,7$100,8Horses:1,"\
		"That Darn Gray Cat,5,won2,Fort Utopia,10,lost3,Count Sheep,9,lost4,Ms Traitour,4,lost5,Real Princess,3,lost6,"\
		"Pa Kettle,5,lost7,Gin Stinger,6,lost"}},
	{2, {"2 25",
		"No Payout: Fort UtopiaInventory:$1,10$5,9$10,9$20,7$100,8Horses:1,That Darn Gray Cat,5,won2,Fort Utopia,"\
		"10,lost3,Count Sheep,9,lost4,Ms Traitour,4,lost5,Real Princess,3,lost6,Pa Kettle,5,lost7,Gin Stinger,6,lost"}},
	{3, {"w 4",
		"Inventory:$1,10$5,9$10,9$20,7$100,8Horses:1,That Darn Gray Cat,5,lost2,Fort Utopia,10,lost3,Count Sheep,9,lost"\
		"4,Ms Traitour,4,won5,Real Princess,3,lost6,Pa Kettle,5,lost7,Gin Stinger,6,lost"}},
	{4, {"4 10.25",
		"Invalid Bet: 10.25Inventory:$1,10$5,9$10,9$20,7$100,8Horses:1,That Darn Gray Cat,5,lost2,Fort Utopia,10,lost3,"\
		"Count Sheep,9,lost4,Ms Traitour,4,won5,Real Princess,3,lost6,Pa Kettle,5,lost7,Gin Stinger,6,lost"}},
	{5, {"T 12",
		"Invalid Command: T 12Inventory:$1,10$5,9$10,9$20,7$100,8Horses:1,That Darn Gray Cat,5,lost2,Fort Utopia,10,lost3,"\
		"Count Sheep,9,lost4,Ms Traitour,4,won5,Real Princess,3,lost6,Pa Kettle,5,lost7,Gin Stinger,6,lost"}},
	{6, {"8 12",
		"Invalid Horse Number: 8Inventory:$1,10$5,9$10,9$20,7$100,8Horses:1,That Darn Gray Cat,5,lost2,Fort Utopia,10,lost3,"\
		"Count Sheep,9,lost4,Ms Traitour,4,won5,Real Princess,3,lost6,Pa Kettle,5,lost7,Gin Stinger,6,lost"}},
	{7, {"4 10000",
		"Insufficient Funds: 40000Inventory:$1,10$5,9$10,9$20,7$100,8Horses:1,That Darn Gray Cat,5,lost2,Fort Utopia,10,lost3,"\
		"Count Sheep,9,lost4,Ms Traitour,4,won5,Real Princess,3,lost6,Pa Kettle,5,lost7,Gin Stinger,6,lost"}},
	{8, {"r",
		"Inventory:$1,10$5,10$10,10$20,10$100,10Horses:1,That Darn Gray Cat,5,lost2,Fort Utopia,10,lost3,Count Sheep,9,lost4,"\
		"Ms Traitour,4,won5,Real Princess,3,lost6,Pa Kettle,5,lost7,Gin Stinger,6,lost"}},
	{9, {"w 8",
		"Invalid Horse Number: 8Inventory:$1,10$5,10$10,10$20,10$100,10Horses:1,That Darn Gray Cat,5,lost2,Fort Utopia,10,lost3,"\
		"Count Sheep,9,lost4,Ms Traitour,4,won5,Real Princess,3,lost6,Pa Kettle,5,lost7,Gin Stinger,6,lost"}},
	{10, {"R 8",
		"Invalid Command: R 8Inventory:$1,10$5,10$10,10$20,10$100,10Horses:1,That Darn Gray Cat,5,lost2,Fort Utopia,10,lost3,"\
		"Count Sheep,9,lost4,Ms Traitour,4,won5,Real Princess,3,lost6,Pa Kettle,5,lost7,Gin Stinger,6,lost"}},
};

namespace TellerHorseRaceTest
{
	TEST_CLASS(TellerHorseRaceTest)
	{
	public:

		TEST_METHOD(TestSetWinner)
		{
			Logger::WriteMessage("Unit test for Horse Race Teller\n");

			// Establish the objects to be tested
			auto logger = std::make_shared<TellerLogger>();
			auto funds = std::make_shared<GameFunds>(logger);
			auto players = std::make_shared<GamePlayers>(funds, logger);

			// Register configuration for fund in the teller machine
			for (auto& fund : cashTestCases) {
				funds->RegisterFund(fund.first, fund.second.first, fund.second.second);
			}

			// Register configuration for houses played in the race
			for (auto& horse : horseTestCases) {
				players->RegisterPlayer(horse.first, horse.second.first, horse.second.second);
			}

			auto teller = std::make_shared<Teller>(players, funds, logger);

			// Step 1 : Verify startup display information
			auto output = logger->GetMessage();
			Assert::AreEqual(true, (output.compare(tellerTestCases.at(0).second) == 0));

			// Step 2 : Simulate customer input and verify corresponding output
			for (auto& testCase : tellerTestCases) {
				logger->ClearMessage();
				teller->ExecuteCommand(testCase.second.first);

				auto output = logger->GetMessage();
				Assert::AreEqual(true, (output.compare(tellerTestCases.at(testCase.first).second) == 0));
			}
		}
	};
}
