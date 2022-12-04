#pragma once

#include <string>

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void PrintLine(std::string) = 0;
};

class TellerLogger : public ILogger
{
public:
    void PrintLine(std::string) override;
};
