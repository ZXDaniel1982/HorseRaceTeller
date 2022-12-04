#include <iostream>
#include <sstream>
#include <regex>
#include "GamePlayers.h"


Horse::Horse(
    int aId,
    std::string aName,
    int aOdd,
    std::shared_ptr<ILogger> aLogger)
    : iId(aId),
    iName(aName),
    iOdd(aOdd),
    iWinner(false),
    iLogger(aLogger)
{}

void Horse::SetWinner(bool aWinner)
{
    iWinner = aWinner;
}

bool Horse::IsWinner()
{
    return iWinner;
}

std::string Horse::GetName()
{
    return iName;
}

int Horse::GetOdd()
{
    return iOdd;
}

void Horse::DisplayInfo()
{
    std::map<bool, std::string> kMap = {
        {false, "lost"},
        {true, "won"}
    };

    std::stringstream ss;
    ss << iId << "," << iName << "," << iOdd << "," << kMap.at(iWinner);
    iLogger->PrintLine(ss.str());
}

GamePlayers::GamePlayers(
    std::shared_ptr<IGameFunds> aGameFunds,
    std::shared_ptr<ILogger> aLogger)
    : iGameFunds(aGameFunds),
    iLogger(aLogger)
{}

void
    GamePlayers::RegisterPlayer(
        int aId,
        std::string aName,
        int aOdd)
{
    auto horsePlayer = std::make_shared<Horse>(aId, aName, aOdd, iLogger);
    if (iGamePlayerList.find(aId) == iGamePlayerList.end()) {
        iGamePlayerList.try_emplace(aId, horsePlayer);
    }
}

void
    GamePlayers::DisplayInfo()
{
    iLogger->PrintLine("Horses:");
    if (iWinner.has_value()) {
        auto winner = iGamePlayerList.at(iWinner.value());
        winner->DisplayInfo();
    }
    for (auto& player : iGamePlayerList) {
        if (!player.second->IsWinner())
        {
            player.second->DisplayInfo();
        }
    }
}

bool
    GamePlayers::SetWinner(std::string aIdIn)
{
    auto id = Convert(aIdIn, std::string("([0-9]+)"));
    if (!id.has_value()) {
        std::stringstream ss;
        ss << "Invalid Horse Number: " << aIdIn;
        iLogger->PrintLine(ss.str());
        return false;
    }

    if (iGamePlayerList.find(id.value()) == iGamePlayerList.end()) {
        std::stringstream ss;
        ss << "Invalid Horse Number: " << aIdIn;
        iLogger->PrintLine(ss.str());
        return false;
    }
    else {
        ClearWinners();
        auto player = iGamePlayerList.at(id.value());
        player->SetWinner(true);
        iWinner = id.value();
        return true;
    }
}

void
    GamePlayers::ClearWinners()
{
    for (auto& player : iGamePlayerList) {
        player.second->SetWinner(false);
    }
}

bool
    GamePlayers::CheckWinner(std::string aIdIn, std::string aBetIn) {
    auto id = Convert(aIdIn, std::string("([0-9]+)"));
    if (!id.has_value()) {
        std::stringstream ss;
        ss << "Invalid Command: " << aIdIn << " " << aBetIn;
        iLogger->PrintLine(ss.str());
        return false;
    }

    auto bet = Convert(aBetIn, std::string("([0-9]+).?([0-9]+)?"));
    if (!bet.has_value()) {
        std::stringstream ss;
        ss << "Invalid Bet: " << aBetIn;
        iLogger->PrintLine(ss.str());
        return false;
    }

    if (iGamePlayerList.find(id.value()) != iGamePlayerList.end()) {
        auto player = iGamePlayerList.at(id.value());
        if (player->IsWinner()) {
            iGameFunds->Payout(player->GetName(), player->GetOdd(), bet.value());
        }
        else {
            std::stringstream ss;
            ss << "No Payout: " << player->GetName();
            iLogger->PrintLine(ss.str());
        }
        return true;
    }
    else {
        return false;
    }
}

std::optional<int>
    GamePlayers::Convert(std::string aString, std::string aQualifier)
{
    std::regex digitRegex(aQualifier);

    std::smatch pieces_match;
    if (!std::regex_search(aString, pieces_match, digitRegex)) {
        return {};
    }

    if (!std::string(pieces_match[2]).empty()) {
        return {};
    }

    std::stringstream ss;
    int value;

    ss << aString;
    ss >> value;

    return value;
}
