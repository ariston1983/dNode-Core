#include "Arduino.h"
#include "dNode-Core.hpp"
#include "Test/test-List.hpp"

void setup(){
  Serial.begin(115200);
  Serial.println();

  TEST_INIT();
  RUN_NODEBASE_TEST();

  // bool _pointer = isPointer<int>();
  // Serial.println(_pointer);

  // bool _res = 
  //   //SCENARIO_IOBJECT_NATIVE_EQUAL()->equal(COMPARE_IOBJECT_NATIVE_EQUAL());
  //   SCENARIO_IOBJECT_NATIVE_EQUAL() == COMPARE_IOBJECT_NATIVE_EQUAL();
  // Serial.println(_res);

  // Serial.println(SCENARIO_IOBJECT_NATIVE_EQUAL()->toString().c_str());
  // Serial.println(COMPARE_IOBJECT_NATIVE_EQUAL()->toString().c_str());
  // Serial.println(SCENARIO_IMODULE_EXEC_NOMETHOD().c_str());
};
void loop(){};
