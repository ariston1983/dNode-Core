#pragma once

#include <string>
#include <map>
#include "node-Helper.hpp"
#include "node-Query.hpp"

class nodeResponse{
  typedef std::map<std::string, std::string> mapData_Type;
private:
  nodeQuery* _query;
  int _code;
  std::string _message;
  mapData_Type* _data;
  mapData_Type* getData(){
    if (this->_data == NULL) this->_data = new mapData_Type();
    return this->_data;
  };
public:
  static nodeResponse* create(nodeQuery* query = NULL, int code = 0, std::string message = "OK"){ return new nodeResponse(query, code, message); };
  nodeResponse(nodeQuery* query = NULL, int code = 0, std::string message = "OK"){};
  nodeResponse* code(int value){ this->_code = value; return this; };
  nodeResponse* message(std::string value){ this->_message = value; return this; };
  nodeResponse* data(std::string key, std::string value){
    if (!key.empty() && key != "") this->getData()->insert(std::make_pair(key, value));
    return this;
  };
  std::string toJSON(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    _obj["code"] = this->_code;
    _obj["message"] = this->_message.c_str();
    for (mapData_Type::iterator _it = this->getData()->begin(); _it != this->getData()->end(); ++_it)
      _obj[_it->first.c_str()] = _it->second.c_str();
    return nodeJSON::stringify(_obj);
  };
};
