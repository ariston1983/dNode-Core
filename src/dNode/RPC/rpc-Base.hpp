#pragma once

#include <string>
#include "ArduinoJson.h"

class IRPCRequest{
private:
  std::string _uid;
  std::string _module;
  std::string _method;
  std::string _params;
protected:
  IRPCRequest(std::string uid, std::string module, std::string method, std::string params = ""){
    this->_uid = uid;
    this->_module = module;
    this->_method = method;
    this->_params = params;
  };
public:
  std::string getUID(){ return this->_uid; };
  std::string getModule(){ return this->_module; };
  std::string getMethod(){ return this->_method; };
  std::string getParams(){ return this->_params; };
};
class IRPCData{
private:
  std::string _kind;
protected:
  IRPCData(std::string kind){
    this->_kind = kind;
  };
public:
  virtual void fillJSON(JsonObject &json){
    json["kind"] = this->_kind.c_str();
  };
};
class IRPCResponse{
private:
  std::string _uid;
  std::string _module;
  std::string _method;
protected:
  IRPCResponse(std::string uid, std::string module, std::string method){
    this->_uid = uid;
    this->_module = module;
    this->_method = method;
  };
public:
  virtual void fillJSON(JsonObject &json){
    json["uid"] = this->_uid.c_str();
    json["module"] = this->_module.c_str();
    json["method"] = this->_method.c_str();
  };
};

class IRPCBase{
public:
  IRPCResponse* exec(IRPCRequest* request){ return NULL; };
};

class IRPCComm{
  typedef IRPCResponse*(*received_Handler)(IRPCBase*, IRPCRequest*);
private:
  IRPCBase* _engine;
  received_Handler _receivedHandler;
protected:
  IRPCResponse* onReceive(IRPCRequest* request){ return this->_receivedHandler(this->_engine, request); };
public:
  ~IRPCComm(){
    delete this->_engine;
  };
  void setRPCEngine(IRPCBase* engine){ this->_engine = engine; };
  void event_OnReceived(received_Handler handler){ this->_receivedHandler = handler; };
};