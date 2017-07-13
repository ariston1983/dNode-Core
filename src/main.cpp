#define __TEST_NAME_LENGTH__ 25

#include "Arduino.h"
#include "dNode-Core.hpp"
#include "Test/test-List.hpp"

void setup(){
  Serial.begin(115200);
  Serial.println();

  TEST_INIT();
  RUN_NODEBASE_TEST();
};
void loop(){};
