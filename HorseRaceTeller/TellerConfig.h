#pragma once

#include "GamePlayers.h"
#include "GameFunds.h"

class TellerConfig
{
public:
    TellerConfig(
        std::string,
        std::shared_ptr<IGamePlayers>,
        std::shared_ptr<IGameFunds>);
private:
    void DecodePlayerConfig(std::string, std::shared_ptr<IGamePlayers>);
    void DecodeFundsConfig(std::string, std::shared_ptr<IGameFunds>);
    int ConvertStringToInt(std::string);
private:
    std::shared_ptr<IGamePlayers> iGamePlayers;
    std::shared_ptr<IGameFunds> iGameFunds;
};
