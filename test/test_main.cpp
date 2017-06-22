#include <string>
#include "Arduino.h"
#include "dNode-Core.h"
#include "unity.h"

void test_make_query(void){
  std::string _module = "TEST_MODULE";
  std::string _subModule = "TEST_SUB_MODULE";
  std::string _command = "TEST_COMMAND";
  std::string _data = "TEST_DATA";
  nodeQuery* _query = new nodeQuery(_module, _subModule, _command, _data);
  TEST_ASSERT_EQUAL_STRING(_module, _query->getModule());
  TEST_ASSERT_EQUAL_STRING(_subModule, _query->getSubModule());
  TEST_ASSERT_EQUAL_STRING(_command, _query->getCommand());
  TEST_ASSERT_EQUAL_STRING(_data, _query->getData());
};

void setup(){
  delay(2000);
  UNITY_BEGIN();
  RUN_TEST(test_make_query);
};
void loop(){};
