#include "Arduino.h"
#include <string>
#include "Test/rpc-Test.hpp"

int getInt(){ return 10; };
std::string getString(){ return "Hello World"; };


void setup(){
  Serial.begin(115200);
  Serial.println();

  TEST_INIT();
  RUN_RPC_TEST();
};
void loop(){};
