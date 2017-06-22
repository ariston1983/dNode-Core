#pragma once

#include <string>
#include "ArduinoJson.h"

std::string stringify(JsonVariant json){
  int _len = json.measureLength()+1;
  char _store[_len];
  json.printTo(_store, _len);
  return std::string(_store);
};
