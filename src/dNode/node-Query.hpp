#pragma once

#include <string>
#include "ArduinoJson.h"

class nodeQuery{
private:
  std::string _module;
  std::string _subModule;
  std::string _command;
  std::string _data;
public:
  static nodeQuery* fromJSON(std::string json){
    std::string _module = "";
    std::string _subModule = "";
    std::string _command = "";
    std::string _data = "";
    if (!json.empty() && json != ""){
      DynamicJsonBuffer _buffer(json.length());
      JsonObject& _obj = _buffer.parseObject(json.c_str());
      _module = _obj["module"].is<const char*>() ? _obj["module"].as<const char*>() : "";
      _subModule = _obj["sub-module"].is<const char*>() ? _obj["sub-module"].as<const char*>() : "";
      _command = _obj["command"].is<const char*>() ? _obj["command"].as<const char*>() : "";
      _data = _obj["data"].is<const char*>() ? _obj["data"].as<const char*>() : "";
    };
    return new nodeQuery(_module, _subModule, _command, _data);
  };
  nodeQuery(std::string module = "", std::string subModule = "", std::string command = "", std::string data = ""){
    this->_module = module;
    this->_subModule = subModule;
    this->_command = command;
    this->_data = data;
  };
  std::string getModule(){ return this->_module; };
  std::string getSubModule(){ return this->_subModule; };
  std::string getCommand(){ return this->_command; };
  bool hasData(){ return !this->_data.empty() && this->_data != ""; };
  int getDataLength(){ return this->hasData() ? this->_data.length() : -1; };
  std::string getData(){ return this->_data; };
  std::string toJSON(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    _obj["module"] = this->_module.c_str();
    _obj["sub-module"] = this->_subModule.c_str();
    _obj["command"] = this->_command.c_str();
    _obj["data"] = this->_data.c_str();
    return nodeJSON::stringify(_obj);
  };
};
