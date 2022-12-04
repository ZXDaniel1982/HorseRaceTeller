#include "Teller.h"
#include "GamePlayers.h"
#include "GameFunds.h"
#include "TellerConfig.h"
#include "TellerLogger.h"

constexpr auto CONFIG_FILE = "Config.xml";

int main()
{
    auto logger = std::make_shared<TellerLogger>();
    auto funds = std::make_shared<GameFunds>(logger);
    auto players = std::make_shared<GamePlayers>(funds, logger);
    auto config = std::make_shared<TellerConfig>(CONFIG_FILE, players, funds);
    auto teller = std::make_shared<Teller>(players, funds, logger);

    teller->Run();
}
