#include <iostream>
#include <sstream>
#include "GameFunds.h"

// Total amount of cashes int the teller machine
int Cash::iTotalFunds = 0;

/**
 * Cashes with specific Denomination
 * 
 * @param[in]   aDenomination - Denomination of the cash
 *              aInventory - The mount of cash of this denomination stored
 *              aMaxInventory - The maximum inventory of this denomination
 *              aLogger - Logger, which reponsible for display output information
 */
Cash::Cash(
    int aDenomination,
    int aInventory,
    int aMaxInventory,
    std::shared_ptr<ILogger> aLogger)
    : iDenomination(aDenomination),
    iInventory(aInventory),
    iMaxInventory(aMaxInventory),
    iDispensing(0),
    iLogger(aLogger)
{
    /**
     * Add new amount of cashes to the machine,
     * update the total funds
     */
    iTotalFunds += iInventory * aDenomination;
}

/**
 * Display how much cash stored for this Denomination
 */
void Cash::DisplayInfo()
{
    std::stringstream ss;
    ss << "$" << iDenomination << "," << iInventory;
    iLogger->PrintLine(ss.str());
}

/**
 * Restore this Denomination cashes back to origin inventory
 */
void Cash::Refill()
{
    iInventory = iMaxInventory;
}

/**
 * Report total funds in the teller machine
 * 
 * @param[out]   iTotalFunds - Total amount of cashes in the machine
 */
int Cash::GetTotalFund()
{
    return iTotalFunds;
}

/**
 * Customer win the game, report how much will the machine pay
 * regard this Denomination
 * 
 * @param[out]   iDispensing - The amount of cashes of this Denomination
 *                             to be payed to customer
 */
int Cash::GetDispensing()
{
    return iDispensing;
}

/**
 * Customer win the game, calculate how much will the machine pay
 * regard this Denomination
 * 
 * @param[in]   aPayOut - The total amount of cashes to be payed to customer
 * @param[out]   The total amount of cashes left be payed to customer
 */
int Cash::Dispensing(int aPayOut)
{
    iDispensing = aPayOut / iDenomination;
    if (iDispensing > iInventory) {
        iDispensing = iInventory;
    }
    iInventory -= iDispensing;
    iTotalFunds -= iDispensing * iDenomination;
    return (aPayOut - (iDispensing * iDenomination));
}


/**
 * Funds stored int the teller machine
 *
 * @param[in]   aLogger - Logger, which reponsible for display output information
 */
GameFunds::GameFunds(std::shared_ptr<ILogger> aLogger)
    : iLogger(aLogger)
{
}


/**
 * Create Cashes based on config resources
 *
 * @param[in]   aDenomination - Denomination of the cash
 *              aInventory - The mount of cash of this denomination stored
 *              aMaxInventory - The maximum inventory of this denomination
 */
void GameFunds::RegisterFund(
        int aDenomination,
        int aInventory,
        int aMaxInventory)
{
    auto cashEntry = std::make_shared<Cash>(aDenomination, aInventory, aMaxInventory, iLogger);
    if (iGameFundList.find(aDenomination) == iGameFundList.end()) {
        iGameFundList.try_emplace(aDenomination, cashEntry);
        iTotalFunds = cashEntry->GetTotalFund();
    }
}

/**
 * Iter all Cash types and display their information
 */
void GameFunds::DisplayInfo()
{
    iLogger->PrintLine("Inventory:");
    for (auto& fund : iGameFundList) {
        fund.second->DisplayInfo();
    }
}

/**
 * Iter all Cash types and restore their inventory back to origin
 */
void GameFunds::Refill()
{
    for (auto& fund : iGameFundList) {
        fund.second->Refill();
    }
}

/**
 * Customer won the game and require for pay out
 * 
 * @param[in]   aName - The name of the horse winner
 *              aOdd - The odd of the horse
 *              aBet - The amount of money customer bet on the winning horse
 */
void GameFunds::Payout(std::string aName, int aOdd, int aBet)
{
    std::stringstream ss;
    int payOut = aOdd * aBet;
    if (payOut > iTotalFunds) {
        ss << "Insufficient Funds: " << payOut;
        iLogger->PrintLine(ss.str());
    }
    else {
        ss << "Payout: " << aName << "," << payOut;
        iLogger->PrintLine(ss.str());
        CalculateDispensing(payOut);
        DisplayDispensing();
    }
}

/**
 * Iter all Cash types and calculate their dispensing
 *
 * @param[in]   aPayOut - The total amount of money customer require to be payed out
 */
void GameFunds::CalculateDispensing(int aPayOut)
{
    std::stringstream ss;
    ss << "Dispensing: " << aPayOut;
    iLogger->PrintLine(ss.str());
    for (auto iter = iGameFundList.rbegin(); iter != iGameFundList.rend(); ++iter) {
        aPayOut = iter->second->Dispensing(aPayOut);
        iTotalFunds = iter->second->GetTotalFund();
    }
}

/**
 * Iter all Cash types and display the Dispensing outcome to customer
 *
 * @param[in]   aPayOut - The total amount of money customer require to be payed out
 */
void
    GameFunds::DisplayDispensing()
{
    for (auto& fund : iGameFundList) {
        auto cash = fund.second;

        std::stringstream ss;
        ss << "$" << fund.first << "," << cash->GetDispensing();
        iLogger->PrintLine(ss.str());
    }
}
