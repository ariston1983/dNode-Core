#pragma once

#include <string>
#include "node-Query.hpp"
#include "node-Response.hpp"

class nodeModule{
private:
protected:
public:
  virtual bool setup(){ return false; };
  virtual bool loop(){ return false; };
  virtual std::vector<rpcMethod> getRPC(){};
  virtual nodeResponse* exec(nodeQuery* query){ return NULL; };
};

class modRPC{
  typedef std::map<std::string, std::string> args_Type;
private:
  std::string _module;
  std::string _method;
  args_Type* _args;
  args_Type* getArgs(){};
public:
  modRPC(std::string module, std::string method){};
  modRPC* addArg(std::string name, std::type){};
  void addToJSON(JsonVariant json){
    std::string _key = this->_module + "." + this->_method;
    JsonObject& _obj = json->createNestedObject(_key.c_str());
    for (args_Type::iterator _it = this->getArgs()->begin(); _it != this->getArgs()->end(); ++_it)
  };
};