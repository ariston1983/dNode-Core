#pragma once

#include <string>
#include "rpc-Base.hpp"

class rpcRequest: public IRPCRequest{
protected:
  rpcRequest(std::string uid, std::string module, std::string method, std::string params = "")
  : IRPCRequest(uid, module, method, params){ };
public:
  static rpcRequest* create(std::string uid, std::string module, std::string method, std::string params = ""){
    return new rpcRequest(uid, module, method, params);
  };
  static rpcRequest* fromJSON(std::string value){
    if (value.empty() || value == "") return NULL;
    DynamicJsonBuffer _buffer(value.length());
    JsonObject& _obj = _buffer.parseObject(value.c_str());
    if (_obj["uid"].is<const char*>() && _obj["module"].is<const char*>() && _obj["method"].is<const char*>()){
      return rpcRequest::create(
        _obj["uid"].as<const char*>(),
        _obj["module"].as<const char*>(),
        _obj["method"].as<const char*>(),
        _obj["params"].is<const char*>() ? _obj["params"].as<const char*>() : ""
      );
    }
    else return NULL;
  };
};
template<class TResponseData>
class rpcResponse: public IRPCResponse{
private:
  IRPCData* _data;
protected:
  rpcResponse(std::string uid, std::string module, std::string method, TResponseData* data)
  : IRPCResponse(uid, module, method){
    this->_data = data;
  };
public:
  rpcResponse<TResponseData>* create(std::string uid, std::string module, std::string method, TResponseData* data){
    return new rpcResponse<TResponseData>(uid, module, method, data);
  };
  virtual void fillJSON(JsonObject &json){
    IRPCResponse::fillJSON(json);
    if (this->_data != NULL){
      JsonObject& _data = json.createNestedObject("data");
      this->_data->fillJSON(_data);
    }
  };
};
class rpcException: public IRPCData{
private:
  std::string _type;
  std::string _message;
  std::string _details;
protected:
  rpcException(std::string type, std::string message, std::string details)
  : IRPCData("exception"){
    this->_type = type;
    this->_message = message;
    this->_details = details;
  };
  virtual void fillJSON(JsonObject& json){
    IRPCData::fillJSON(json);
    json["type"] = this->_type.c_str();
    json["message"] = this->_message.c_str();
    json["details"] = this->_details.c_str();
  };
};
