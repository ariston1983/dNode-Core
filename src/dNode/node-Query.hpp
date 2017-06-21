#pragma once

#include <string>
#include "ArduinoJson.h"

class nodeQuery{
private:
  std::string _module;
  std::string _subModule;
  std::string _command;
  std::string _args;
public:
  nodeQuery* parse(std::string json){
    std::string _module = "";
    std::string _subModule = "";
    std::string _command = "";
    std::string _args = "";
    if (!json.empty() && json != ""){
      DynamicJsonBuffer _buffer(json.length());
      JsonObject& _obj = _buffer.parseObject(json.c_str());
      _module = _obj["module"].is<const char*>() ? _obj["module"].as<const char*>() : "";
      _subModule = _obj["sub"].is<const char*>() ? _obj["sub"].as<const char*>() : "";
      _command = _obj["command"].is<const char*>() ? _obj["command"].as<const char*>() : "";
      _args = _obj["args"].is<const char*>() ? _obj["args"].as<const char*>() : "";
    };
    return new nodeQuery(_module, _subModule, _command, _args);
  };
  nodeQuery(std::string module = "", std::string subModule = "", std::string command = "", std::string args = ""){
    this->_module = module;
    this->_subModule = subModule;
    this->_command = command;
    this->_args = args;
  };
  std::string getModule(){ return this->_module; };
  std::string getSubModule(){ return this->_subModule; };
  std::string getCommand(){ return this->_command; };
  bool hasArgs(){ return !this->_args.empty() && this->_args != ""; };
  int argsLength(){ return this->_args.length(); };
  std::string getArgs(){ return this->_args; };
};
