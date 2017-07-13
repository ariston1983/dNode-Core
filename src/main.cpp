#define __TEST_NAME_LENGTH__ 25
#define __TEST_LOG_STEPS__

#include "Arduino.h"
#include "dNode-Core.hpp"
#include "Test/test-List.hpp"

using namespace dNode;

void setup(){
  Serial.begin(115200);
  Serial.println();

  TEST_INIT();
  // Test::Base::EXECUTE();
  Test::Module::Base::EXECUTE();
};
void loop(){};
