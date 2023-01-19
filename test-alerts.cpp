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

// TEST_CASE("check and alert") {
//   AlertHandler alertHandlerTest;
//   double tempInC = 30;
//   BatteryCharacter BatteryCharacterTest{PASSIVE_COOLING, "GM"};

//   checkAndAlert(TO_CONTROLLER,BatteryCharacterTest, tempInC);
// }

// TEST_CASE("check alert message strings") {
//   BreachType breachType = NORMAL;
//   controllerAlert controllerAlert(breachType, 0xfeed);

//   REQUIRE(controllerAlert.buildAlertMessageString() == "0xfeed : NORNAL\n");

// }