#pragma once

#include <string>
#include <vector>
#include "FS.h"
#include "ArduinoJson.h"

std::string stringify(JsonVariant json){
  int _len = json.measureLength()+1;
  char _store[_len];
  json.printTo(_store, _len);
  return std::string(_store);
};

typedef std::vector<std::string> dirList_Type;

bool prepFS(bool format = false){ return false; };
dirList_Type* dir(std::string root = "/", bool recursive = false){ return NULL; };
bool fileExists(std::string file, std::string dir = "/"){ return false; };
std::string readFile(std::string file, std::string dir = "/"){ return ""; };
bool writeFile(std::string content, std::string file, std::string dir = "/"){ return false; };
bool deleteFile(std::string file, std::string dir = "/"){ return false; };
