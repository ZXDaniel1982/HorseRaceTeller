#include <string>
#include <iostream>
#include "TellerLogger.h"

using namespace std;

void TellerLogger::PrintLine(std::string aOuput)
{
	iOutput.append(aOuput);
	cout << aOuput << endl;
}
