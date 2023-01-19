#pragma once

#include <string>
#include <map>
#include <set>
#include <vector>
typedef enum {
  PASSIVE_COOLING,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING
} CoolingType;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH
} BreachType;

typedef enum {
  TO_CONTROLLER,
  TO_EMAIL
} AlertTarget;

typedef struct {
  CoolingType coolingType;
  char brand[48];
} BatteryCharacter;

typedef std::pair<double, double> temperatureLimitsPair;
typedef std::map<CoolingType, temperatureLimitsPair> temperatureLimitsMap;

static const temperatureLimitsMap tempLimitsMap{
  {PASSIVE_COOLING,    std::make_pair(0,35)},
  {MED_ACTIVE_COOLING, std::make_pair(0,40)},
  {HI_ACTIVE_COOLING,  std::make_pair(0,45)}
  };

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC);

class IFtargetAlert
{
  public:
  virtual std::string  buildAlertMessageString() = 0;

  protected:
  BreachType breachType;
  unsigned short header = 0x0;
  std::string eMail;
};

class EmailAlert: public IFtargetAlert
{
  public:
  EmailAlert(BreachType lbreachType, std::string eMailValue){
                breachType= lbreachType;
                eMail = eMailValue;}
  std::string  buildAlertMessageString();
};

class controllerAlert: public IFtargetAlert
{
  public:
  controllerAlert(BreachType lbreachType, unsigned short headerValue){
                  breachType= lbreachType; 
                  header = headerValue;}
  std::string  buildAlertMessageString();
};

class AlertHandler
{
  public:
  AlertHandler() = default;
  IFtargetAlert* targetAlert; 
  void setTargetAlert(IFtargetAlert* ifTargetAlert){targetAlert = ifTargetAlert;}

  BreachType inferBreach(double value, double lowerLimit, double upperLimit);
  temperatureLimitsPair getTemperatureLimits(CoolingType coolingType);
  BreachType getTemperatureBreach(CoolingType coolingType, double temperatureInC);
  void sendAlertToTarget(std::string alertMessage);
};