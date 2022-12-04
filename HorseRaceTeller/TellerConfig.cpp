#include <iostream>
#include <sstream>
#include "TellerConfig.h"
#include "tinyxml2.h"

using namespace tinyxml2;

TellerConfig::TellerConfig(std::string aConfigFile,
	std::shared_ptr<IGamePlayers> aGamePlayers,
	std::shared_ptr<IGameFunds> aGameFunds)
{
	DecodePlayerConfig(aConfigFile, aGamePlayers);
	DecodeFundsConfig(aConfigFile, aGameFunds);
}

int
TellerConfig::ConvertStringToInt(std::string aString)
{
	std::stringstream ss;
	int value;

	ss << aString;
	ss >> value;

	return value;
}

void
TellerConfig::DecodePlayerConfig(std::string aConfigFile, std::shared_ptr<IGamePlayers> aGamePlayers)
{
	XMLDocument doc;
	doc.LoadFile(aConfigFile.c_str());
	XMLElement* pRootElement = doc.RootElement();

	if (NULL != pRootElement) {
		XMLElement* horsesElement = pRootElement->FirstChildElement("horses");

		if (NULL != horsesElement) {
			XMLElement* horseElement = horsesElement->FirstChildElement("horse");
			while (horseElement) {
				int id, odd;
				std::string name;

				XMLElement* idElement = horseElement->FirstChildElement("id");
				if (idElement == NULL) {
					horseElement = horseElement->NextSiblingElement("horse");
					continue;
				}
				id = ConvertStringToInt(idElement->GetText());

				XMLElement* nameElement = horseElement->FirstChildElement("name");
				if (nameElement == NULL) {
					horseElement = horseElement->NextSiblingElement("horse");
					continue;
				}
				name = std::string(nameElement->GetText());

				XMLElement* oddElement = horseElement->FirstChildElement("odd");
				if (oddElement == NULL) {
					horseElement = horseElement->NextSiblingElement("horse");
					continue;
				}
				odd = ConvertStringToInt(oddElement->GetText());
				
				aGamePlayers->RegisterPlayer(id, name, odd);

				horseElement = horseElement->NextSiblingElement("horse");
			}
		}
	}
}

void
TellerConfig::DecodeFundsConfig(std::string aConfigFile, std::shared_ptr<IGameFunds> aGameFunds)
{
	XMLDocument doc;
	doc.LoadFile(aConfigFile.c_str());
	XMLElement* pRootElement = doc.RootElement();

	if (NULL != pRootElement) {
		XMLElement* cashesElement = pRootElement->FirstChildElement("cashes");

		if (NULL != cashesElement) {
			XMLElement* cashElement = cashesElement->FirstChildElement("cash");
			while (cashElement) {
				int denomination, inventory, maxInventory;

				XMLElement* denominationElement = cashElement->FirstChildElement("Denomination");
				if (denominationElement == NULL) {
					cashElement = cashElement->NextSiblingElement("cash");
					continue;
				}
				denomination = ConvertStringToInt(denominationElement->GetText());

				XMLElement* inventoryElement = cashElement->FirstChildElement("Inventory");
				if (inventoryElement == NULL) {
					cashElement = cashElement->NextSiblingElement("cash");
					continue;
				}
				inventory = ConvertStringToInt(inventoryElement->GetText());

				XMLElement* maxInventoryElement = cashElement->FirstChildElement("MaxInventory");
				if (maxInventoryElement == NULL) {
					cashElement = cashElement->NextSiblingElement("cash");
					continue;
				}
				maxInventory = ConvertStringToInt(maxInventoryElement->GetText());

				aGameFunds->RegisterFund(denomination, inventory, maxInventory);

				cashElement = cashElement->NextSiblingElement("cash");
			}
		}
	}
}