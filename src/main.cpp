#include "Arduino.h"
#include <string>
#include "ArduinoJson.h"
#include "dNode-Core.hpp"
#include "Test/rpc-Test.hpp"

int i_scenario(){ return 10; };
int i_compare(){ return 1; };
std::string s_scenario(){ return "Hello"; };


void setup(){
  Serial.begin(115200);
  Serial.println();

  TEST_INIT();
  RUN_TEST<int>("TEST_COMPARE_WITH_FUNCTION")
    ->scenario(&i_scenario)
    ->checkIf(TEST_GREATER_OR_EQUAL)
    ->with(&i_compare)
    ->execute();
  RUN_TEST<std::string>("TEST_COMPARE_WITH_VALUE")
    ->scenario(&s_scenario)
    ->checkIf(TEST_EQUAL)
    ->to("Hello")->execute();
  RUN_TEST<std::string>("RPC_RESPONSE_CUSTOM_DATA")
    ->scenario(&TEST_SCENARIO_RPC_RESPONSE_CUSTOM_DATA)
    ->checkIf(TEST_EQUAL)
    ->with(&TEST_COMPARER_RPC_RESPONSE_CUSTOM_DATA)
    ->execute();
};
void loop(){};
