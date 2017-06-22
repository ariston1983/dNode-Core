#pragma once

#include <string>
#include "ArduinoJson.h"
#include "node-Helper.hpp"

class nodeConfig{
private:
  std::string _module;
  std::string configFile(){ return this->_module+".json"; };
protected:
  virtual bool parseJSON(std::string json){
    DynamicJsonBuffer _buffer(json.length());
    JsonObject& _obj = _buffer.parseObject(json.c_str());
    return _obj["module"].is<const char*>() && _obj["module"].as<const char*>() == this->_module;
  };
  void init(){ this->load(true); };
public:
  nodeConfig(std::string module){};
  bool load(bool reset = false){
    if (fileExists(this->configFile(), std::string("/config")))
      return this->parseJSON(readFile(this->configFile(), std::string("/config")));
    else{
      if (reset) this->reset();
      return false;
    }
  };
  bool save(){ return writeFile(this->toJSON(), this->configFile(), std::string("/config")); };
  virtual void reset(){ };
  bool remove(){ return deleteFile(this->configFile(), std::string("/config")); };
  virtual std::string toJSON(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    _obj["module"] = this->_module.c_str();
    return stringify(_obj);
  };
};
