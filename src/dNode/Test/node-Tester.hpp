#pragma once

#include "Arduino.h"
#include <string>
#include "../Logger/node-Logger.hpp"
#include "test-Unit.hpp"

void TEST_LOGGER(std::string message, int code, std::string data){
  std::string _print = "";
  switch (code){
    case 1: _print = message + " RESULT [" + data + "]"; break;
    case 2: _print = "..." + message; break;
    case 0:
    default: _print = message; break;
  };
  Serial.print(_print.c_str());
  if (code != 2) { Serial.print(" [HEAP: "); Serial.print(ESP.getFreeHeap()); Serial.print("]"); }
  Serial.println();
};
void TEST_INIT(){
  nodeLogger::init(&TEST_LOGGER);
  LOG("TEST INITIALIZED");
};
template<typename TTest>
testUnit<TTest>* RUN_TEST(std::string name){
  return new testUnit<TTest>(name);
};