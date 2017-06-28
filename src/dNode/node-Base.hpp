#pragma once

#include <string>
#include <vector>
#include <map>
#include "node-Helper.hpp"

class IJSONSupport{
public:
  virtual bool fromJSON(std::string json){ return false; };
  virtual void fillJSON(JsonVariant json){ };
  virtual std::string toJSON(){ return ""; };
};
class IResultData{};
class IResult{};
class RemoteMethod: public IJSONSupport{
  typedef std::map<std::string, std::string> param_Type;
private:
  std::string _module;
  std::string _name;
  param_Type* _params;
  param_Type* getParams(){
    if (this->_params == NULL) this->_params = new param_Type();
    return this->_params;
  };
  std::string getUniqueName(){
    return this->_module + "." + this->_name;
  };
public:
  RemoteMethod(std::string module, std::string name){
    this->_module = module;
    this->_name = name;
  };
  RemoteMethod* addParam(std::string name, std::string type){
    this->getParams()->insert(std::make_pair(name, type));
    return this;
  };
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonArray>())
      this->fillJSON(json.as<JsonArray>().createNestedObject());
    else if (json.is<JsonObject>()){
      JsonObject& _remoteMethod = json.as<JsonObject>().createNestedObject(this->getUniqueName().c_str());
      JsonArray& _params = _remoteMethod.createNestedArray("params");
      for (param_Type::iterator _it = this->getParams()->begin(); _it != this->getParams()->end(); ++_it){
        JsonObject& _param = _params.createNestedObject();
        _param["name"] = _it->first.c_str();
        _param["type"] = _it->second.c_str();
      }
    }
  };
  virtual std::string toJSON(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    fillJSON(_obj);
    std::string _json = nodeJSON::stringify(_obj);
    delete_if_pointer(_obj);
    return _json;
  };
};
RemoteMethod* createRemoteMethod(std::string module, std::string name){
  return new RemoteMethod(module, name);
};
class IExecutable{
public:
  virtual std::vector<RemoteMethod*>* getRemoteMethods(){ return NULL; };
  virtual IResult* execute(std::string method, std::string params){ return NULL; };
};
class IModule: public IExecutable{
private:
  std::string _name;
  std::string _version;
public:
  IModule(std::string name, std::string version){
    this->_name = name;
    this->_version = version;
  };
  virtual bool setup(){ return false; };
  virtual bool loop(){ return false; };
};