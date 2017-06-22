#pragma once

#include <string>
#include "node-Query.hpp"
#include "node-Response.hpp"

class nodeModule{
private:
public:
  virtual bool setup(){ return false; };
  virtual bool loop(){ return false; };
  virtual nodeResponse* exec(nodeQuery* query){ return NULL; };
};
