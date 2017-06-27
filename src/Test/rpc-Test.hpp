#pragma once

#include <string>
#include "../dNode-Core.hpp"

std::string TEST_SCENARIO_RPC_RESPONSE_CUSTOM_DATA(){
  rpcResponse* _response = rpcResponse::create<rpcNative<const char*>>("uid", "module", "method", rpcNative<const char*>::create("string", "Hello World"));
  DynamicJsonBuffer _buffer(512);
  JsonObject& _obj = _buffer.createObject();
  _response->fillJSON(_obj);
  return nodeJSON::stringify(_obj);
};
std::string TEST_COMPARER_RPC_RESPONSE_CUSTOM_DATA(){
  rpcResponse* _response = rpcResponse::create("uid", "module", "method");
  _response->native<const char*>("string", "Hello World");
  DynamicJsonBuffer _buffer(512);
  JsonObject& _obj = _buffer.createObject();
  _response->fillJSON(_obj);
  return nodeJSON::stringify(_obj);
};