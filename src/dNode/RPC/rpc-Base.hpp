#pragma once

#include <string>
#include "ArduinoJson.h"
#include "../node-Base.hpp"

/**************************************************************
 * RPC package base
 **************************************************************/
class RPCRequest: public IJSONSupport{
  typedef std::map<std::string, std::string> param_Type;
private:
  std::string _uid;
  std::string _module;
  std::string _method;
  param_Type* _params;
  std::string getUniqueName(){
    return this->_module + "." + this->_method;
  };
public:
  RPCRequest(std::string uid, std::string module, std::string method){
    this->_uid = uid;
    this->_module = module;
    this->_method = method;
  };
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonArray>()) this->fillJSON(json.as<JsonArray>().createNestedObject());
    else if (json.is<JsonObject>()){
      JsonObject& _obj = json.as<JsonObject>();
      _obj["uid"] = this->_uid.c_str();
      JsonArray& _params = _obj.createNestedArray(strdup(this->getUniqueName().c_str()));
      for (param_Type::iterator _it = this->getParams()->begin(); _it != this->getParams()->end(); ++_it){
        JsonObject& _param = _params.createNestedObject();
        _param[_it->first.c_str()] = _it->second.c_str();
      }
    };
  };
  virtual std::string toJSON(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    this->fillJSON(_obj);
    return nodeJSON::stringify(_obj);
  };
  std::string getUID(){ return this->_uid; };
  std::string getModule(){ return this->_module; };
  std::string getMethod(){ return this->_method; };
  param_Type* getParams(){
    if (this->_params == NULL) this->_params = new param_Type();
    return this->_params;
  };
  bool hasParam(std::string name){
    param_Type::iterator _it = this->getParams()->find(name);
    return _it != this->getParams()->end();
  };
  std::string paramType(std::string name){
    param_Type::iterator _it = this->getParams()->find(name);
    return (_it != this->getParams()->end()) ? _it->second : "";
  };
  void setParam(std::string name, std::string type){
    param_Type::iterator _it = this->getParams()->find(name);
    if (_it != this->getParams()->end()) _it->second = type;
    else this->getParams()->insert(std::make_pair(name, type));
  };
};
class RPCResponse: public IResult{
private:
  std::string _uid;
public:
  RPCResponse(std::string uid, std::string module, std::string method)
  : IResult(module, method){
    this->_uid = uid;
  };
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonArray>()) this->fillJSON(json.as<JsonArray>().createNestedObject());
    else if (json.is<JsonObject>()){
      JsonObject& _obj = json.as<JsonObject>();
      _obj["uid"] = this->_uid.c_str();
      JsonObject& _data = _obj.createNestedObject(strdup(this->getUniqueName().c_str()));
      if (this->_data != NULL) this->_data->fillJSON(_data);
    }
  };
};
/**************************************************************/

/**************************************************************
 * RPC node base
 **************************************************************/
// class IRPCBase{
// public:
//   IRPCResponse* exec(IRPCRequest* request){ return NULL; };
// };
/**************************************************************/

/**************************************************************
 * RPC communicator base
 **************************************************************/
// class IRPCComm{
//   typedef IRPCResponse*(*received_Handler)(IRPCBase*, IRPCRequest*);
// private:
//   IRPCBase* _engine;
//   received_Handler _receivedHandler;
// protected:
//   IRPCResponse* onReceive(IRPCRequest* request){ return this->_receivedHandler(this->_engine, request); };
// public:
//   ~IRPCComm(){
//     delete_if_pointer(_engine);
//   };
//   void setRPCEngine(IRPCBase* engine){ this->_engine = engine; };
//   void event_OnReceived(received_Handler handler){ this->_receivedHandler = handler; };
// };
/**************************************************************/