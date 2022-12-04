#include "pch.h"
#include "CppUnitTest.h"

#include <memory>

#include "../HorseRaceTeller/TellerLogger.h"
#include "../HorseRaceTeller/GameFunds.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TellerHorseRaceTest
{
	TEST_CLASS(TellerHorseRaceTest)
	{
	public:
		
		TEST_METHOD(TestSetWinner)
		{
			//TellerLogger logger;
			//ILogger * loggerPtr = &logger;
			//std::shared_ptr<ILogger> loggerPtr = &logger;
			//GameFunds funds(&logger);
			//auto funds = std::make_shared<GameFunds>(logger);
			//std::shared_ptr<TellerLogger> logger = std::make_shared<TellerLogger>();
			std::string input("w 2");
		}
	};
}
