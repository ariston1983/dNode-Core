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