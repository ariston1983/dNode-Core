#pragma once

#include <string>
#include "../dNode-Core.hpp"

std::string TEST_SCENARIO_RPC_RESPONSE_CUSTOM_DATA(){
  rpcResponse* _response = rpcResponse::create<rpcNative<const char*>>("uid", "module", "method", rpcNative<const char*>::create("string", "Hello World"));
  DynamicJsonBuffer _buffer(512);
  JsonObject& _obj = _buffer.createObject();
  _response->fillJSON(_obj);
  std::string _result = nodeJSON::stringify(_obj);
  delete_if_pointer(_obj);
  return _result;
};
std::string TEST_COMPARER_RPC_RESPONSE_CUSTOM_DATA(){
  rpcResponse* _response = rpcResponse::create("uid", "module", "method");
  _response->native<const char*>("string", "Hello World");
  DynamicJsonBuffer _buffer(512);
  JsonObject& _obj = _buffer.createObject();
  _response->fillJSON(_obj);
  std::string _result = nodeJSON::stringify(_obj);
  delete_if_pointer(_obj);
  return _result;
};
void RUN_RPC_TEST(){
  RUN_TEST<std::string>("RPC_RESPONSE_CUSTOM_DATA")
    ->scenario(&TEST_SCENARIO_RPC_RESPONSE_CUSTOM_DATA)
    ->checkIf(TEST_EQUAL)
    ->with(&TEST_COMPARER_RPC_RESPONSE_CUSTOM_DATA)
    ->execute();
};