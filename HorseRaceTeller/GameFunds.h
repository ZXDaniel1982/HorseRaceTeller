#pragma once

#include <memory>
#include <map>

#include "TellerLogger.h"

class ICash
{
public:
    virtual ~ICash() = default;
    virtual void DisplayInfo() = 0;
    virtual void Refill() = 0;
    virtual int GetTotalFund() = 0;
    virtual int Dispensing(int) = 0;
    virtual int GetDispensing() = 0;
};

class Cash : public ICash
{
public:
    Cash(int, int, int);
public:
    void DisplayInfo() override;
    void Refill() override;
    int GetTotalFund() override;
    int Dispensing(int) override;
    int GetDispensing() override;
private:
    int         iDenomination;
    int         iInventory;
    int         iMaxInventory;
    int         iDispensing;

    static int  iTotalFunds;
};

class IGameFunds
{
public:
    virtual ~IGameFunds() = default;
    virtual void RegisterFund(int, int, int) = 0;
    virtual void DisplayInfo() = 0;
    virtual void Refill() = 0;
    virtual void Payout(std::string, int, int) = 0;
};

class GameFunds : public IGameFunds
{
public:
    GameFunds(std::shared_ptr<ILogger>);
public:
    void RegisterFund(int, int, int) override;
    void DisplayInfo() override;
    void Refill() override;
    void Payout(std::string, int, int) override;
private:
    void DisplayDispensing(int);
    void CalculateDispensing(int);
    void DisplayDispensing();
private:
    std::map<int, std::shared_ptr<ICash>> iGameFundList;
    std::shared_ptr<ILogger> iLogger;
    int iTotalFunds;
    int iDispensing;
};
