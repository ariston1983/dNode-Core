#pragma once

#include "Arduino.h"
#include <string>
#include <vector>
#include <map>
#include "node-Helper.hpp"

class IObject{
public:
  friend bool operator==(IObject& lhs, IObject& rhs){ return lhs.equal(&rhs); };
  virtual bool equal(IObject* obj){ return this->toString() == obj->toString(); };
  virtual std::string toString(){ return ""; };
};

class IJSONSupport: public IObject{
public:
  virtual std::string toString(){ return this->toJSON(); };
  virtual bool fromJSON(std::string json){ return false; };
  virtual void fillJSON(JsonVariant json){ };
  virtual std::string toJSON(){ return ""; };
};

class IResultData: public IJSONSupport{
private:
  std::string _kind;
protected:
  IResultData(std::string kind){ this->_kind = kind; };
public:
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonObject>()){
      JsonObject& _obj = json.as<JsonObject>();
      _obj["kind"] = this->_kind.c_str();
    }
  };
  virtual std::string toJSON(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    this->fillJSON(_obj);
    return nodeJSON::stringify(_obj);
  };
};
template<typename T>
class NativeData: public IResultData{
private:
  std::string _type;
  T _value;
protected:
  NativeData(std::string type, T value): IResultData("native"){
    this->_type = type;
    this->_value = value;
  };
public:
  static NativeData<T>* create(std::string type, T value){
    if (isNative<T>()){ return new NativeData(type, value); }
    else return NULL;
  };
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonObject>()){
      IResultData::fillJSON(json);
      JsonObject& _obj = json.as<JsonObject>();
      _obj["type"] = this->_type.c_str();
      _obj["value"] = this->_value;
    }
  };
};
template<>
class NativeData<std::string>: public IResultData{
private:
  std::string _type;
  std::string _value;
protected:
  NativeData(std::string type, std::string value): IResultData("native"){
    this->_type = type;
    this->_value = value;
  };
public:
  static NativeData<std::string>* create(std::string type, std::string value){
    return new NativeData<std::string>("string", value);
  };
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonObject>()){
      IResultData::fillJSON(json);
      JsonObject& _obj = json.as<JsonObject>();
      _obj["type"] = this->_type.c_str();
      _obj["value"] = this->_value.c_str();
    }
  };
};
template<typename T>
NativeData<T>* nativeData(std::string type, T value){
  return NativeData<T>::create(type, value);
};
class ExceptionData: public IResultData{
private:
  std::string _type;
  std::string _message;
  std::string _details;
public:
  ExceptionData(std::string type, std::string message, std::string details)
  : IResultData("exception"){
    this->_type = type;
    this->_message = message;
    this->_details = details;
  };
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonObject>()){
      IResultData::fillJSON(json);
      JsonObject& _obj = json.as<JsonObject>();
      _obj["type"] = this->_type.c_str();
      _obj["message"] = this->_message.c_str();
      _obj["details"] = this->_details.c_str();
    }
  };
  virtual std::string toJSON(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    this->fillJSON(_obj);
    return nodeJSON::stringify(_obj);
  };
};
ExceptionData* exceptionData(std::string type, std::string message, std::string details = ""){
  return new ExceptionData(type, message, details);
};

class IResult: public IJSONSupport{
private:
  std::string _module;
  std::string _method;
protected:
  IResultData* _data;
  std::string getUniqueName(){
    return this->_module + "." + this->_method;
  };
public:
  IResult(std::string module, std::string method){
    this->_module = module;
    this->_method = method;
    this->_data = NULL;
  };
  std::string getModule(){ return this->_module; };
  std::string getMethod(){ return this->_method; };
  IResult* setData(IResultData* data){
    if (data != NULL) this->_data = data;
    return this;
  };
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonArray>()) this->fillJSON(json.as<JsonArray>().createNestedObject());
    else if (json.is<JsonObject>()){
      JsonObject& _data = json.as<JsonObject>().createNestedObject(strdup(this->getUniqueName().c_str()));
      if (this->_data != NULL) this->_data->fillJSON(_data);
    };
  };
  virtual std::string toJSON(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    this->fillJSON(_obj);
    return nodeJSON::stringify(_obj);
  };
};

class MethodInfo: public IJSONSupport{
  typedef std::map<std::string, std::string> param_Type;
private:
  std::string _module;
  std::string _name;
  param_Type* _params;
  param_Type* getParams(){
    if (this->_params == NULL) this->_params = new param_Type();
    return this->_params;
  };
  std::string getUniqueName(){
    return this->_module + "." + this->_name;
  };
public:
  MethodInfo(std::string module, std::string name){
    this->_module = module;
    this->_name = name;
  };
  std::string getModule(){ return this->_module; };
  std::string getName(){ return this->_name; };
  MethodInfo* addParam(std::string name, std::string type){
    this->getParams()->insert(std::make_pair(name, type));
    return this;
  };
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonArray>()) this->fillJSON(json.as<JsonArray>().createNestedObject());
    else if (json.is<JsonObject>()){
      JsonArray& _MethodInfo = json.as<JsonObject>().createNestedArray(strdup(this->getUniqueName().c_str()));
      for (param_Type::iterator _it = this->getParams()->begin(); _it != this->getParams()->end(); ++_it){
        JsonObject& _param = _MethodInfo.createNestedObject();
        _param["name"] = _it->first.c_str();
        _param["type"] = _it->second.c_str();
      }
    }
  };
  virtual std::string toJSON(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    this->fillJSON(_obj);
    std::string _json = nodeJSON::stringify(_obj);
    delete_if_pointer(_obj);
    return _json;
  };
};
MethodInfo* methodInfo(std::string module, std::string name){
  return new MethodInfo(module, name);
};

typedef std::vector<MethodInfo*> listMethod_Type;
class IModule{
private:
  std::string _name;
  std::string _version;
protected:
  typedef IResult*(IModule::*execHandler)(std::string params);
  virtual execHandler getHandler(std::string method){ return NULL; };
  IModule(std::string name, std::string version = "1.0"){
    this->_name = name;
    this->_version = version;
  };
public:
  IResult* execute(std::string method, std::string params){
    execHandler _handler = this->getHandler(method);
    if (_handler != NULL) return (this->*_handler)(params);
    else return (new IResult(this->_name, method))->setData(exceptionData("MethodNotFound", "Method [" + method + "] not found"));
  };
  virtual listMethod_Type* getMethods(){ return NULL; };
  virtual bool setup(){ return false; };
  virtual bool loop(){ return false; };
};