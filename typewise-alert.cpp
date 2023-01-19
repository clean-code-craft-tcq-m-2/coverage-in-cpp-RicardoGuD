#include "typewise-alert.h"
#include <stdio.h>
#include <iostream>

BreachType AlertHandler::inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) 
{
  AlertHandler alertHandler;
  BreachType breachType = alertHandler.getTemperatureBreach(batteryChar.coolingType, temperatureInC);

  switch(alertTarget) {
    case TO_CONTROLLER:
    {
      controllerAlert controllerAlert(breachType, 0xfeed);
      alertHandler.setTargetAlert(&controllerAlert);
      break;
    }
    case TO_EMAIL:
    {
      EmailAlert emailAlert(breachType, "a.b@c.com");
      alertHandler.setTargetAlert(&emailAlert);
      break;
    }
  }
  
  std::string alertMessage = alertHandler.targetAlert->buildAlertMessageString();
  alertHandler.sendAlertToTarget(alertMessage);
}

std::string controllerAlert::buildAlertMessageString()
{
  std::string controllerMessage = "%x : %x\n", header, breachType;
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

void AlertHandler::sendAlertToTarget(std::string alertMessage){
  std::cout << alertMessage;
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
