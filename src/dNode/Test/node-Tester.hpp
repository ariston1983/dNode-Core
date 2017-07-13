#pragma once

#include "Arduino.h"
#include <string>
#include "../Logger/node-Logger.hpp"
#include "unit-Test.hpp"

using namespace dNode::UnitTest;

void TEST_LOGGER(std::string message, int code, std::string data){
  std::string _print = "";
  switch (code){
    case 1:
      if (data == "GOOD") _print = message + " RESULT [[[" + data + "]]]";
      else _print = message + " RESULT !!!" + data + "!!!";
      break;
    case 2: _print = "..." + message; break;
    case 0:
    default: _print = message; break;
  };
  if (code != 2){
    Serial.print(_print.c_str());
    if (code == 0){ Serial.print(" [HEAP: "); Serial.print(ESP.getFreeHeap()); Serial.print("]"); }
    Serial.println();
    if (code == 1) Serial.println();
  }
  else{
    #ifdef __TEST_LOG_STEPS__
    Serial.print(_print.c_str());
    Serial.println();
    #endif
  }
};
void TEST_INIT(){
  nodeLogger::init(&TEST_LOGGER);
  LOG("TEST INITIALIZED");
};
template<typename T>
UnitTest<T>* RUN_TEST(std::string name){
  return new UnitTest<T>(name);
};