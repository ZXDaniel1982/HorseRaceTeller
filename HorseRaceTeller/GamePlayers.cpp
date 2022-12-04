#include <iostream>
#include <sstream>
#include <regex>
#include "GamePlayers.h"

/**
 * Horse the played the racing game
 *
 * @param[in]   aId - The id of the horse
 *              aName - The name of the horse
 *              aOdd - The odd of the horse
 *              aLogger - Logger, which reponsible for display output information
 */
Horse::Horse(
    int aId,
    std::string aName,
    int aOdd,
    std::shared_ptr<ILogger> aLogger)
    : iId(aId),
    iName(aName),
    iOdd(aOdd),
    iWinner((aId == 1) ? true : false),
    iLogger(aLogger)
{}

/**
 * Set this horse as the winner
 * 
 * @param[in]   aWinner - True means this horse is winner, false means opposite
 */
void Horse::SetWinner(bool aWinner)
{
    iWinner = aWinner;
}

/**
 * Report if this horse is a winner
 *
 * @param[out]   iWinner - True means this horse is winner, false means opposite
 */
bool Horse::IsWinner()
{
    return iWinner;
}

/**
 * Report the name of his horse
 *
 * @param[out]   iName - The name of his horse
 */
std::string Horse::GetName()
{
    return iName;
}

/**
 * Report the odd of his horse
 *
 * @param[out]   iOdd - The odd of his horse
 */
int Horse::GetOdd()
{
    return iOdd;
}

/**
 * Display the information of his horse
 */
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

/**
 * Manage the players in the racing game
 */
GamePlayers::GamePlayers(
    std::shared_ptr<IGameFunds> aGameFunds,
    std::shared_ptr<ILogger> aLogger)
    : iGameFunds(aGameFunds),
    iLogger(aLogger)
{}

/**
 * Create a horse based on config resources
 *
 * @param[in]   aId - The id of the horse
 *              aName - The name of the horse
 *              aOdd - The odd of the horse
 */
void GamePlayers::RegisterPlayer(
        int aId,
        std::string aName,
        int aOdd)
{
    auto horsePlayer = std::make_shared<Horse>(aId, aName, aOdd, iLogger);
    if (iGamePlayerList.find(aId) == iGamePlayerList.end()) {
        iGamePlayerList.try_emplace(aId, horsePlayer);
    }
}

/**
 * Iter all horses and display their information
 */
void GamePlayers::DisplayInfo()
{
    iLogger->PrintLine("Horses:");
    for (auto& player : iGamePlayerList) {
        player.second->DisplayInfo();
    }
}

/**
 * Set the winner horse and unset all other horses as not winner
 * 
 * @param[in]   aIdIn - The id of the horse the inputed
 */
bool GamePlayers::SetWinner(std::string aIdIn)
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

/**
 * Reset all horses as not winner
 */
void GamePlayers::ClearWinners()
{
    for (auto& player : iGamePlayerList) {
        player.second->SetWinner(false);
    }
}

/**
 * Customer would like to check if the horse he bet is a winner
 * 
 * @param[in]   aIdIn - The id of the horse
 *              aBetIn - The amount of money customer bet on the horse
 */
bool GamePlayers::CheckWinner(std::string aIdIn, std::string aBetIn) {
    /**
     * Read the id of the horse from customer's input
     */
    auto id = Convert(aIdIn, std::string("([0-9]+)"));
    if (!id.has_value()) {
        std::stringstream ss;
        ss << "Invalid Command: " << aIdIn << " " << aBetIn;
        iLogger->PrintLine(ss.str());
        return false;
    }

    /**
     * Read the bet of the horse from customer's input
     */
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
            /**
             * If the horse customer bet on is a winner, goto payout process
             */
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
        std::stringstream ss;
        ss << "Invalid Horse Number: " << aIdIn;
        iLogger->PrintLine(ss.str());
        return false;
    }
}

std::optional<int> GamePlayers::Convert(std::string aString, std::string aQualifier)
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
