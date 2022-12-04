#pragma once

#include <list>
#include <set>
#include <string>
#include <memory>
#include <optional>

#include "GameFunds.h"

class IAnimal
{
public:
    virtual ~IAnimal() = default;
    virtual void DisplayInfo() = 0;
    virtual void SetWinner(bool) = 0;
    virtual bool IsWinner() = 0;
    virtual int GetOdd() = 0;
    virtual std::string GetName() = 0;
};

class Horse : public IAnimal
{
public:
    Horse(
        int,
        std::string,
        int,
        std::shared_ptr<ILogger>);
public:
    void DisplayInfo() override;
    void SetWinner(bool) override;
    bool IsWinner() override;
    int GetOdd() override;
    std::string GetName() override;
private:
    int         iId;
    std::string iName;
    int         iOdd;
    bool        iWinner;
    std::shared_ptr<ILogger> iLogger;
};

class IGamePlayers
{
public:
    virtual ~IGamePlayers() = default;
    virtual void RegisterPlayer(int, std::string, int) = 0;
    virtual void DisplayInfo() = 0;
    virtual bool SetWinner(std::string) = 0;
    virtual bool CheckWinner(std::string, std::string) = 0;
};

class GamePlayers : public IGamePlayers
{
public:
    GamePlayers(
        std::shared_ptr<IGameFunds>,
        std::shared_ptr<ILogger>);
public:
    void RegisterPlayer(int, std::string, int) override;
    void DisplayInfo() override;
    bool SetWinner(std::string) override;
    bool CheckWinner(std::string, std::string) override;
private:
    void ClearWinners();
    std::optional<int> Convert(std::string, std::string);
private:
    std::map<int, std::shared_ptr<IAnimal>> iGamePlayerList;
    std::shared_ptr<IGameFunds> iGameFunds;
    std::optional<int> iWinner;
    std::shared_ptr<ILogger> iLogger;
};