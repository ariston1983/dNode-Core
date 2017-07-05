#pragma once

#include "Arduino.h"
#include <string>
#include <vector>
#include <map>
#include "node-Helper.hpp"

/****************************************************************************
 * Base interfaces
 ****************************************************************************/
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
  virtual std::string toJSON(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    this->fillJSON(_obj);
    return nodeJSON::stringify(_obj);
  };
};

/****************************************************************************
 * Invoker interfaces
 ****************************************************************************/
class InvokerArgument{
  enum ArgumentType{
    TYPE_INVALID,
    TYPE_BOOL,
    TYPE_INTEGER,
    TYPE_FLOATING,
    TYPE_CHARS
  };
  union ArgumentValue{
    bool asBool;
    unsigned long asInteger;
    float asFloat;
    const char* asChars;
  };
private:
  std::string _name;
  ArgumentType _type;
  ArgumentValue _value;
public:
  InvokerArgument(){
    this->_name = "";
    this->_type = TYPE_INVALID;
  };
  InvokerArgument(std::string name, JsonVariant value){
    this->_name = name;
    this->_type = TYPE_INVALID;
    if (isJSONBool(value)) this->set<bool>(value.as<bool>());
    else if (isJSONInteger(value)) this->set<unsigned int>(value.as<unsigned int>());
    else if (isJSONFloat(value)) this->set<float>(value.as<float>());
    else if (isJSONChars(value)) this->set<const char*>(value.as<const char*>());
  };
  template<typename T>
  InvokerArgument(std::string name, T value, typename enableIf<isBool<T>::value ||
    isInteger<T>::value ||
    isFloat<T>::value ||
    isChars<T>::value ||
    isString<T>::value>::type* = 0){
    this->_name = name;
    this->_type = TYPE_INVALID;
    this->set<T>(value);
  };
  static InvokerArgument& invalid(){
    static InvokerArgument instance;
    return instance;
  };
  std::string getName(){ return this->_name; };
  bool valid(){ return !this->_name.empty() && this->_name != "" && this->_type != TYPE_INVALID; };
  template<typename T>
  T as(typename enableIf<isBool<T>::value>::type* = 0){
    if (this->_type == TYPE_BOOL) return T(this->_value.asBool);
    else return 0;
  };
  template<typename T>
  T as(typename enableIf<isInteger<T>::value>::type* = 0){
    if (this->_type == TYPE_INTEGER) return T(this->_value.asInteger);
    else return 0;
  };
  template<typename T>
  T as(typename enableIf<isFloat<T>::value>::type* = 0){
    if (this->_type == TYPE_FLOATING) return T(this->_value.asFloat);
    else return 0;
  };
  template<typename T>
  T as(typename enableIf<isChars<T>::value>::type* = 0){
    if (this->_type == TYPE_CHARS) return T(this->_value.asChars);
    else return "";
  };
  template<typename T>
  T as(typename enableIf<isString<T>::value>::type* = 0){
    if (this->_type == TYPE_CHARS) return std::string(this->_value.asChars);
    else return "";
  };
  template<typename T>
  bool set(T value, typename enableIf<isBool<T>::value>::type* = 0){
    if (this->_type == TYPE_INVALID || this->_type == TYPE_BOOL){
      this->_type = TYPE_BOOL;
      this->_value.asBool = static_cast<bool>(value);
      return true;
    }
    else return false;
  };
  template<typename T>
  bool set(T value, typename enableIf<isInteger<T>::value>::type* = 0){
    if (this->_type == TYPE_INVALID || this->_type == TYPE_INTEGER){
      this->_type = TYPE_INTEGER;
      this->_value.asInteger = static_cast<unsigned int>(value);
      return true;
    }
    else return false;
  };
  template<typename T>
  bool set(T value, typename enableIf<isFloat<T>::value>::type* = 0){
    if (this->_type == TYPE_INVALID || this->_type == TYPE_FLOATING){
      this->_type = TYPE_FLOATING;
      this->_value.asFloat = static_cast<float>(value);
      return true;
    }
    else return false;
  };
  template<typename T>
  bool set(T value, typename enableIf<isChars<T>::value>::type* = 0){
    if (this->_type == TYPE_INVALID || this->_type == TYPE_CHARS){
      this->_type = TYPE_CHARS;
      this->_value.asChars = static_cast<const char*>(value);
      return true;
    }
    else return false;
  };
  template<typename T>
  bool set(T value, typename enableIf<isString<T>::value>::type* = 0){
    if (this->_type == TYPE_INVALID || this->_type == TYPE_CHARS){
      this->_type = TYPE_CHARS;
      this->_value.asChars = static_cast<std::string>(value).c_str();
      return true;
    }
    else return false;
  };
  template<typename T>
  typename enableIf<isBool<T>::value ||
    isInteger<T>::value ||
    isFloat<T>::value ||
    isString<T>::value, InvokerArgument&>::type operator=(const T& value){
    this->set<T>(value);
    return *this;
  };
  template<typename T>
  typename enableIf<isChars<T>::value, InvokerArgument&>::type operator=(const T* value){
    this->set<T>(value);
    return *this;
  };
  template<typename T>
  operator T(){
    return this->as<T>();
  };
};
class Invoker{
  typedef std::map<std::string, InvokerArgument*> args_Type;
private:
  std::string _module;
  std::string _method;
  args_Type* _args;
  args_Type* getArgs(){
    if (this->_args == NULL) this->_args = new args_Type();
    return this->_args;
  };
public:
  Invoker(std::string module, std::string method){
    this->_module = module;
    this->_method = method;
    this->_args = NULL;
  };
  ~Invoker(){
    for (auto _it = this->getArgs()->begin(); _it != this->getArgs()->end(); ++_it){
      delete _it->second;
      this->getArgs()->erase(_it);
    }
    delete this->_args;
  };
  static Invoker& invalid(){
    static Invoker instance("", "");
    return instance;
  };
  std::string getModule(){ return this->_module; };
  std::string getMethod(){ return this->_method; };
  std::vector<std::string>* getParamNames(){
    std::vector<std::string>* _names = new std::vector<std::string>();
    for (args_Type::iterator _it = this->getArgs()->begin(); _it != this->getArgs()->end(); ++_it)
      _names->push_back(_it->first);
    return _names;
  };
  template<typename T>
  Invoker& add(std::string name, T value){
    args_Type::iterator _it = this->getArgs()->find(name);
    if (_it != this->getArgs()->end()){
      if (_it->second->set<T>(value)) return *this;
      else return Invoker::invalid();
    }
    this->getArgs()->insert(std::make_pair(name, new InvokerArgument(name, value)));
    return *this;
  };
  int count(){ return this->getArgs()->size(); };
  bool has(std::string name){ return this->getArgs()->find(name) != this->getArgs()->end(); };
  InvokerArgument& get(std::string name){
    args_Type::iterator _it = this->getArgs()->find(name);
    if (_it != this->getArgs()->end()) return *_it->second;
    else InvokerArgument::invalid();
  };
  InvokerArgument& operator[](std::string name){ return this->get(name); };
};
class IExecArgs: public IJSONSupport{
public:
  IExecArgs(){ };
  virtual bool fromJSON(std::string json){ return true; };
  virtual bool fromJSON(JsonVariant json){ return true; };
  virtual void fillJSON(JsonVariant json){ };
  template<typename T>
  T* as(){ return static_cast<T*>(this); }
};
class ExecMeta: public IJSONSupport{
private:
  std::string _module;
  std::string _method;
  IExecArgs* _args;
public:
  ExecMeta(IExecArgs* args){
    this->_args = NULL;
    if (args != NULL) this->_args = args;
  };
  virtual bool fromJSON(std::string json){
    DynamicJsonBuffer _buffer(json.length());
    JsonVariant _var = _buffer.parseObject(json.c_str());
    if (_var.success() && _var.is<JsonObject>()){
      JsonObject& _obj = _var.as<JsonObject>();
      if (!_obj.containsKey("module")) return false;
      this->_module = (const char*)_obj["module"];
      if (!_obj.containsKey("method")) return false;
      this->_method = (const char*)_obj["method"];
      if (_obj.containsKey("args") && _obj["args"].is<JsonObject>() && this->_args != NULL)
        return this->_args->fromJSON(_obj["args"].as<JsonObject>());
      else return true;
    }
    else return false;
  };
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonArray>()) this->fillJSON(json.as<JsonArray>().createNestedObject());
    else if (json.is<JsonObject>()){
      JsonObject& _obj = json.as<JsonObject>();
      _obj["module"] = strdup(this->_module.c_str());
      _obj["method"] = strdup(this->_method.c_str());
      JsonObject& _args = _obj.createNestedObject("args");
      if (this->_args != NULL) this->_args->fillJSON(_args);
    };
  };
  std::string getModule(){ return this->_module; };
  std::string getMethod(){ return this->_method; };
  IExecArgs* getArgs(){ return this->_args; };
  template<typename T>
  T* getArgsAs(){ return this->_args->as<T>(); };
};
template<typename TArgs>
ExecMeta* execMeta(std::string json){
  ExecMeta* _meta = new ExecMeta(new TArgs());
  if (_meta->fromJSON(json)) return _meta;
  else return NULL;
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

/************************************************************************************
 * Module interfaces
 ************************************************************************************/
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
  typedef IResultData*(IModule::*execHandler)(IExecArgs* args);
  virtual execHandler getHandler(std::string method){ return NULL; };
  IModule(std::string name, std::string version = "1.0"){
    this->_name = name;
    this->_version = version;
  };
  virtual IResult* makeResult(std::string method){
    return (new IResult(this->_name, method));
  };
public:
  IResult* execute(ExecMeta* execInfo){
    if (execInfo->getModule() != this->_name)
      return this->makeResult(execInfo->getMethod())->setData(exceptionData("InvalidModule", "Invalid module [" + execInfo->getModule() + "] to execute"));
    execHandler _handler = this->getHandler(execInfo->getMethod());
    if (_handler != NULL)
      return this->makeResult(execInfo->getMethod())->setData((this->*_handler)(execInfo->getArgs()));
    else
      return this->makeResult(execInfo->getMethod())->setData(exceptionData("MethodNotFound", "Method [" + execInfo->getMethod() + "] not found"));
  };
  virtual listMethod_Type* getMethods(){ return NULL; };
  virtual bool setup(){ return false; };
  virtual bool loop(){ return false; };
};