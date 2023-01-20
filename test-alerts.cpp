#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("infers the breach according to limits") {
  AlertHandler alertHandlerTest;
  REQUIRE(alertHandlerTest.inferBreach(12, 20, 30) == TOO_LOW);
  REQUIRE(alertHandlerTest.inferBreach(25, 20, 30) == NORMAL);
  REQUIRE(alertHandlerTest.inferBreach(35, 20, 30) == TOO_HIGH);
}

TEST_CASE("get temperature limits") {
  AlertHandler alertHandlerTest;
  double lowerLimit = alertHandlerTest.getTemperatureLimits(PASSIVE_COOLING).first;
  double upperLimit = alertHandlerTest.getTemperatureLimits(PASSIVE_COOLING).second;
  REQUIRE(lowerLimit == 0);
  REQUIRE(upperLimit == 35);

  lowerLimit = alertHandlerTest.getTemperatureLimits(MED_ACTIVE_COOLING).first;
  upperLimit = alertHandlerTest.getTemperatureLimits(MED_ACTIVE_COOLING).second;
  REQUIRE(lowerLimit == 0);
  REQUIRE(upperLimit == 40);

  lowerLimit = alertHandlerTest.getTemperatureLimits(HI_ACTIVE_COOLING).first;
  upperLimit = alertHandlerTest.getTemperatureLimits(HI_ACTIVE_COOLING).second;
  REQUIRE(lowerLimit == 0);
  REQUIRE(upperLimit == 45);
}

TEST_CASE("get temperature breach according to the cooling type") {
  AlertHandler alertHandlerTest;
  REQUIRE(alertHandlerTest.getTemperatureBreach(PASSIVE_COOLING, 30) == NORMAL);
  REQUIRE(alertHandlerTest.getTemperatureBreach(PASSIVE_COOLING, -5) == TOO_LOW);
  REQUIRE(alertHandlerTest.getTemperatureBreach(PASSIVE_COOLING, 40) == TOO_HIGH);

  REQUIRE(alertHandlerTest.getTemperatureBreach(MED_ACTIVE_COOLING, 30) == NORMAL);
  REQUIRE(alertHandlerTest.getTemperatureBreach(MED_ACTIVE_COOLING, -5) == TOO_LOW);
  REQUIRE(alertHandlerTest.getTemperatureBreach(MED_ACTIVE_COOLING, 45) == TOO_HIGH);

  REQUIRE(alertHandlerTest.getTemperatureBreach(HI_ACTIVE_COOLING, 30) == NORMAL);
  REQUIRE(alertHandlerTest.getTemperatureBreach(HI_ACTIVE_COOLING, -5) == TOO_LOW);
  REQUIRE(alertHandlerTest.getTemperatureBreach(HI_ACTIVE_COOLING, 50) == TOO_HIGH);
}

TEST_CASE("check controller alert message strings") {
  BreachType breachType = NORMAL;
  controllerAlert controllerAlertNormal(breachType, 0xfeed);
  REQUIRE(controllerAlertNormal.buildAlertMessageString() == "feed : 0\n");

  breachType = TOO_LOW;
  controllerAlert controllerAlertLow(breachType, 0xa01b);
  REQUIRE(controllerAlertLow.buildAlertMessageString() == "a01b : 1\n");

  breachType = TOO_HIGH;
  controllerAlert controllerAlerthigh(breachType, 0xffff);
  REQUIRE(controllerAlerthigh.buildAlertMessageString() == "ffff : 2\n");
 }

 TEST_CASE("check email alert message strings") {
  BreachType breachType = TOO_LOW;
  EmailAlert eMailAlertLow(breachType, "targetmail@domain.com");
  REQUIRE(eMailAlertLow.buildAlertMessageString() == "To: targetmail@domain.com\nHi, the temperature is too low\n");

  breachType = TOO_HIGH;
  EmailAlert eMailAlertHigh(breachType, "targetmail@domain.com");
  REQUIRE(eMailAlertHigh.buildAlertMessageString() == "To: targetmail@domain.com\nHi, the temperature is too high\n");

  breachType = NORMAL;
  EmailAlert eMailAlertNormal(breachType, "targetmail@domain.com");
  REQUIRE(eMailAlertNormal.buildAlertMessageString() == "");
 }

 TEST_CASE("create the target object") {
  AlertHandler alertHandlerTest;
  AlertTarget alertTarget = TO_CONTROLLER;
  BreachType breachType = TOO_LOW;
  REQUIRE(alertHandlerTest.createAlertTarget(alertTarget, breachType) == true);

  alertTarget = TO_EMAIL;
  REQUIRE(alertHandlerTest.createAlertTarget(alertTarget, breachType) == true);

  alertTarget = INVALID;
  REQUIRE(alertHandlerTest.createAlertTarget(alertTarget, breachType) == false);
 }

//  TEST_CASE("check temperature level alert proccesing") {
//   AlertTarget alertTarget = TO_CONTROLLER;
//   char dummychar = {};
//   BatteryCharacter BatteryCharacterTest = {PASSIVE_COOLING, dummychar};
//   double tempInC = 30;
//   REQUIRE(checkAndAlert(alertTarget, BatteryCharacterTest, tempInC) == true);
//  }