#include "Arduino.h"
#include <string>
#include "ArduinoJson.h"
#include "dNode-Core.hpp"

int i_scenario(){ return 10; };
int i_compare(){ return 1; };
std::string s_scenario(){ return "Hello"; };


void setup(){
  Serial.begin(115200);
  Serial.println();

  TEST_INIT();
  RUN_TEST<int>("Simple")->scenario(&i_scenario)->checkIf(TEST_GREATER_OR_EQUAL)->with(&i_compare)->execute();
  RUN_TEST<std::string>("Simple")->scenario(&s_scenario)->checkIf(TEST_EQUAL)->to("Hello")->execute();

  //rpcNative<const char*>* _data = rpcNative<const char*>::create("string", "Hello World");
  rpcNative<bool>* _data = rpcNative<bool>::create("bool", false);
  //IRPCData* _cast = static_cast<IRPCData*>(_data);
  rpcResponse<rpcNative<bool>>* _resp = rpcResponse<rpcNative<bool>>::create("uid", "module", "method", _data);
  //DynamicJsonBuffer _buffer(512);
  //JsonObject& _obj = _buffer.createObject();
  //_resp->fillJSON(_obj);
  //_obj.printTo(Serial);
};
void loop(){};
