#pragma once

#include <string>

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void PrintLine(std::string) = 0;
    virtual void ClearMessage() = 0;
    virtual std::string GetMessage() = 0;
};

class TellerLogger : public ILogger
{
public:
    void PrintLine(std::string) override;
    void ClearMessage() override {
        iOutput.clear();
    }
    std::string GetMessage() override {
        return iOutput;
    }
private:
    std::string iOutput;
};

