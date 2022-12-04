#pragma once

#include <list>
#include <set>
#include <string>
#include <memory>

#include "GamePlayers.h"
#include "GameFunds.h"

class Teller
{
public:
    Teller(std::shared_ptr<IGamePlayers>,
        std::shared_ptr<IGameFunds>,
        std::shared_ptr<ILogger>);
public:
    void Run();
    void Exit();
    void Listen();
    void DisplayInfo();
    void ExecuteCommand(std::string);
private:
    std::shared_ptr<IGamePlayers> iGamePlayers;
    std::shared_ptr<IGameFunds> iGameFunds;
    bool iKeepRunning;
    std::shared_ptr<ILogger> iLogger;
};