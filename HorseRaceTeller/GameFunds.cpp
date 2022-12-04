#include <iostream>
#include <sstream>
#include "GameFunds.h"

int Cash::iTotalFunds = 0;

Cash::Cash(
    int aDenomination,
    int aInventory,
    int aMaxInventory)
    : iDenomination(aDenomination),
    iInventory(aInventory),
    iMaxInventory(aMaxInventory),
    iDispensing(0)
{
    iTotalFunds += iInventory * aDenomination;
}

void
    Cash::DisplayInfo()
{
    std::cout << "$" << iDenomination << "," << iInventory << std::endl;
}

void
    Cash::Refill()
{
    iInventory = iMaxInventory;
}

int
    Cash::GetTotalFund()
{
    return iTotalFunds;
}

int
    Cash::GetDispensing()
{
    return iDispensing;
}

int
    Cash::Dispensing(int aPayOut)
{
    iDispensing = aPayOut / iDenomination;
    if (iDispensing > iInventory) {
        iDispensing = iInventory;
    }
    iInventory -= iDispensing;
    iTotalFunds -= iDispensing * iDenomination;
    return (aPayOut - (iDispensing * iDenomination));
}

GameFunds::GameFunds(std::shared_ptr<ILogger> aLogger)
    : iLogger(aLogger)
{
}

void
    GameFunds::RegisterFund(
        int aDenomination,
        int aInventory,
        int aMaxInventory)
{
    auto cashEntry = std::make_shared<Cash>(aDenomination, aInventory, aMaxInventory);
    if (iGameFundList.find(aDenomination) == iGameFundList.end()) {
        iGameFundList.try_emplace(aDenomination, cashEntry);
        iTotalFunds = cashEntry->GetTotalFund();
    }
}

void
    GameFunds::DisplayInfo()
{
    iLogger->PrintLine("Inventory:");
    for (auto& fund : iGameFundList) {
        fund.second->DisplayInfo();
    }
}

void
    GameFunds::Refill()
{
    for (auto& fund : iGameFundList) {
        fund.second->Refill();
    }
}

void
    GameFunds::Payout(std::string aName, int aOdd, int aBet)
{
    std::stringstream ss;
    int payOut = aOdd * aBet;
    if (payOut > iTotalFunds) {
        ss << "Insufficient Funds: " << payOut;
        iLogger->PrintLine(ss.str());
    }
    else {
        ss << "Payout: " << aName << "," << payOut << ", total " << iTotalFunds;
        iLogger->PrintLine(ss.str());
        CalculateDispensing(payOut);
        DisplayDispensing();
    }
}

void
    GameFunds::CalculateDispensing(int aPayOut)
{
    std::stringstream ss;
    ss << "Dispensing: " << aPayOut;
    iLogger->PrintLine(ss.str());
    for (auto iter = iGameFundList.rbegin(); iter != iGameFundList.rend(); ++iter) {
        aPayOut = iter->second->Dispensing(aPayOut);
        iTotalFunds = iter->second->GetTotalFund();
    }
}

void
    GameFunds::DisplayDispensing()
{
    for (auto& fund : iGameFundList) {
        auto cash = fund.second;
        std::cout << "$" << fund.first << "," << cash->GetDispensing() << std::endl;
    }
}
