#include "typewise-alert.h"
#include <stdio.h>
#include <iostream>

AlertHandler::AlertHandler(){}
AlertHandler::~AlertHandler(){}
EmailAlert::~EmailAlert(){}
controllerAlert::~controllerAlert(){}

BreachType AlertHandler::inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

std::string controllerAlert::buildAlertMessageString()
{
  std::stringstream ssHeader;
  std::stringstream ssbreachType;
  ssHeader << std::hex << header;
  ssbreachType << std::hex << breachType;
  std::string controllerMessage = ssHeader.str() +" : " + ssbreachType.str() + "\n";
  return controllerMessage;
}

std::string EmailAlert::buildAlertMessageString()
{
  std::string eMailMessage = "";
  if(breachType == TOO_LOW){
    eMailMessage = "To: " + eMail + "\n" + "Hi, the temperature is too low\n";
  }
  if(breachType == TOO_HIGH){
    eMailMessage = "To: " + eMail + "\n" + "Hi, the temperature is too high\n";
  }
  return eMailMessage;
}

bool AlertHandler::sendAlertToTarget(std::string alertMessage){
  bool isSuccess = true;
  std::cout << alertMessage;
  return isSuccess;
}

temperatureLimitsPair AlertHandler::getTemperatureLimits(CoolingType coolingType)
{
  return tempLimitsMap.at(coolingType);
}

BreachType AlertHandler::getTemperatureBreach(CoolingType coolingType, double temperatureInC)
{
  std::pair<double, double> temperatureLimits = getTemperatureLimits(coolingType);
  double lowerLimit = temperatureLimits.first;
  double upperLimit = temperatureLimits.second;
  return inferBreach(temperatureInC, lowerLimit, upperLimit);
}

bool AlertHandler::createAlertTarget(AlertTarget alertTarget, BreachType breachType)
{
  bool isSuccess = false;
  switch(alertTarget) 
    {
      case TO_CONTROLLER:
      {
        controllerAlert controllerAlert(breachType, 0xfeed);
        this->setTargetAlert(&controllerAlert);
        isSuccess = true;
        break;
      }
      case TO_EMAIL:
      {
        EmailAlert emailAlert(breachType, "a.b@c.com");
        this->setTargetAlert(&emailAlert);
        isSuccess = true;
        break;
      } 
      default:
      {}
    }
    return isSuccess;
}

bool checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) 
{
  bool isSuccess = false;
  AlertHandler alertHandler;
  BreachType breachType = alertHandler.getTemperatureBreach(batteryChar.coolingType, temperatureInC);

  alertHandler.createAlertTarget(alertTarget, breachType);
  std::string alertMessage = alertHandler.targetAlert->buildAlertMessageString();
  isSuccess = alertHandler.sendAlertToTarget(alertMessage);
  return isSuccess;
}
