#pragma once

#include <string>
#include <map>
#include "ArduinoJson.h"
#include "node-Factory.hpp"
#include "node-Module.hpp"

class rpcRequest{
private:
  std::string _uid;
  std::string _module;
  std::string _method;
  std::string _params;
  rpcRequest(std::string uid; std::string module, std::string method, std::string params){
    this->_uid = uid;
    this->_module = module;
    this->_method = method;
    this->_params = params;
  };
public:
  static rpcRequest* fromJSON(std::string json){
    DynamicJsonBuffer _buffer(json.length());
    JsonObject& _obj = _buffer.parseObject(json.c_str());
    return new rpcRequest(
      _obj["uid"].is<const char*>() ? _obj["uid"].as<const char*>() : "",
      _obj["module"].is<const char*>() ? _obj["module"].as<const char*>() : "",
      _obj["method"].is<const char*>() ? _obj["method"].as<const char*>() : "",
      _obj["params"].is<JsonObject&>() ? nodeJSON::stringify(_obj["params"].as<JsonObject&>()) : ""
    );
  };
  std::string getUID(){ return this->_uid; };
  std::string getModule(){ return this->_module; };
  std::string getMethod(){ return this->_method; };
  std::string getParams(){ return this->_params; };
  nodeModule* createModule(){ return nodeFactory::create(this->getModule()); };
};
class rpcResponseBase{
private:
  std::string _uid;
  std::string _module;
  std::string _method;
public:
  rpcResponseBase(std::string uid, std::string module, std::string method){
    this->_uid = uid;
    this->_module = module;
    this->_method = method;
  };
  virtual int length(){
    int _len = 0;
    if (!this->_uid.empty()) _len += this->_uid.length();
    if (!this->_module.empty()) _len += this->_module.length();
    if (!this->_method.empty()) _len += this->_method.length();
    if (!this->_return.empty()) _len += this->_return.length();
    return _len;
  };
  virtual std::string toJSON(){
    DynamicJsonBuffer _buffer(this->length());
    JsonObject& _obj = _buffer.createObject();
    _obj["uid"] = this->_uid.c_str();
    _obj["module"] = this->_module.c_str();
    _obj["method"] = this->_method.c_str();
    return nodeJSON::stringify(_obj);
  };
};
class rpcException: public rpcResponseBase{
private:
  std::string _message;
public:
  rpcException(std::string uid, std::string module, std::string method, std::string message)
  : rpcResponseBase(uid, module, method){
    this->_message = message;
  };
  virtual int length(){
    int _len = rpcResponseBase::length();
    _len += std::string("exception").length();
    if (!this->_message.empty()) _len += this->_message.length();
    return _len;
  };
  virtual std::string toJSON(){
    std::string _baseJSON = rpcResponseBase::toJSON();
    DynamicJsonBuffer _buffer(this->length());
    JsonObject& _obj = _buffer.parseObject(_baseJSON.c_str());
    _obj["type"] = "exception";
    _obj["message"] = this->_message.c_str();
    return nodeJSON::stringify(_obj);
  };
};
template<typename TValue>
class rpcReturn: public rpcResponseBase{
private:
  std::string _type;
  TValue _value;
public:
  rpcReturn(std::string uid, std::string module, std::string method, std::string type, TValue value)
  : rpcResponseBase(uid, module, method){
    this->_type = type;
    this->_value = value;
  };
  virtual int length(){
    int _len = rpcResponseBase::length();
    if (!this->_type.empty()) _len += this->_type.length();
    return _len;
  };
  virtual std::string toJSON(){
    std::string _baseJSON = rpcResponseBase::toJSON();
    DynamicJsonBuffer _buffer(this->length());
    JsonObject& _obj = _buffer.parseObject(_baseJSON.c_str());
    _obj["type"] = this->_type.c_str();
    _obj["value"] = this->_value;
  };
};
class rpcComm : public nodeModule{
protected:
  void onReceived(rpcRequest* req){};
public:
  void event_Received(){};

};

class nodeRPC{
  class rpcMethod{
    typedef std::map<std::string, std::string> args_Type;
  private:
    std::string _module;
    std::string _method;
    std::string _return;
    args_Type* _args;
    args_Type* getArgs(){
      if (this->_args == NULL) this->_args = new args_Type();
      return this->_args;
    };
  public:
    rpcMethod(std::string module, std::string method, std::string ret = "void"){
      this->_module = module;
      this->_method = method;
      this->_return = ret;
    };
    rpcMethod* addArgs(std::string name, std::string type){
      if (!name.empty() && name != "" && !type.empty() && type != "")
        this->getArgs()->insert(std::make_pair(name, type));
      return this;
    };
    void fillJSON(JsonVariant &json){
      std::string _methodKey = this->_module + "." + this->_method;
      JsonObject& _method = json.createNestedObject(_methodKey.c_str());
      _method["return"] = this->_return.c_str();
      JsonObject& _args = _method.createNestedObject("args");
      for (args_Type::iterator _it = this->getArgs()->begin(); _it != this->getArgs()->end(); ++_it)
        _args[_it->first.c_str()] = _args[_it->second.c_str()];
    };
  };
  rpcComm* _communicator;
private:
  static rpcMethod* rpcMethodMeta(std::string module, std::string method, std::string ret = "void"){
    return new rpcMethod(module, method, ret);
  };
  nodeRPC(){};
};