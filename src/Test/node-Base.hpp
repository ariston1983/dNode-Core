#pragma once

#include "../dNode/node-Base.hpp"
#include "../dNode/Test/node-Tester.hpp"

IResult* SCENARIO_IOBJECT_NATIVE_EQUAL(){
  return (new IResult("TEST_MOD", "equal"))->setData(nativeData<int>("int", (int)100));
};
bool EVAL_IOBJECT_NATIVE_EQUAL(TestComparer op, IResult* value, IResult* target){
  IResult* _comp = (new IResult("TEST_MOD", "equal"))->setData(nativeData<int>("int", (int)100));
  switch(op){
    case TEST_EQUAL: return value->equal(_comp); break;
    default: return false;
  };
};

class JsonSupport: public IJSONSupport{
private:
  std::string _prop;
public:
  std::string getProp(){ return this->_prop; };
  virtual bool fromJSON(std::string json){
    DynamicJsonBuffer _buffer(json.length());
    JsonVariant _var = _buffer.parseObject(json.c_str());
    if (_var.is<JsonObject>()){
      JsonObject& _obj = _var.as<JsonObject>();
      this->_prop = (const char*)_obj["prop"];
    }
    else return false;
  };
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonArray>()) this->fillJSON(json.as<JsonArray>().createNestedObject());
    else if (json.is<JsonObject>()){
      JsonObject& _obj = json.as<JsonObject>();
      _obj["prop"] = this->_prop.c_str();
    };
  };
  virtual std::string toJSON(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    this->fillJSON(_obj);
    return nodeJSON::stringify(_obj);
  };
};
std::string JSON_SOURCE = "{\"prop\":\"Hello\"}";
std::string SCENARIO_JSONSUPPORT_FROM_JSON(){
  JsonSupport* _js = new JsonSupport();
  _js->fromJSON(JSON_SOURCE);
  return _js->getProp();
};
std::string SCENARIO_JSONSUPPORT_TO_JSON(){
  JsonSupport* _js = new JsonSupport();
  _js->fromJSON(JSON_SOURCE);
  return _js->toJSON();
};
std::string SCENARIO_JSONSUPPORT_FILL_OBJECT(){
  JsonSupport* _js = new JsonSupport();
  _js->fromJSON(JSON_SOURCE);
  DynamicJsonBuffer _buffer(512);
  JsonObject& _obj = _buffer.createObject();
  _js->fillJSON(_obj);
  return nodeJSON::stringify(_obj);
};
std::string SCENARIO_JSONSUPPORT_FILL_ARRAY(){
  JsonSupport* _js = new JsonSupport();
  _js->fromJSON(JSON_SOURCE);
  DynamicJsonBuffer _buffer(512);
  JsonArray& _arr = _buffer.createArray();
  _js->fillJSON(_arr);
  return nodeJSON::stringify(_arr);
};

std::string EXEC_JSON_SOURCE = "{\"module\":\"execTester\",\"method\":\"calculate\",\"args\":{\"x\":20,\"y\":10}}";
class CalculateArgs: public IExecArgs{
private:
  int _x;
  int _y;
public:
  CalculateArgs(): IExecArgs(){ };
  virtual bool fromJSON(JsonVariant json){
    if (json.is<JsonObject>()){
      JsonObject& _obj = json.as<JsonObject>();
      if (!_obj.containsKey("x")) return false;
      this->_x = _obj["x"];
      if (!_obj.containsKey("y")) return false;
      this->_y = _obj["y"];
      return true;
    }
    else return false;
  };
  virtual bool fromJSON(std::string json){
    DynamicJsonBuffer _buffer(json.length());
    JsonVariant _var = _buffer.parseObject(json.c_str());
    if (_var.success() && _var.is<JsonObject>()) this->fromJSON(_var);
    else return false;
  };
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonObject>()){
      JsonObject& _obj = json.as<JsonObject>();
      _obj["x"] = this->_x;
      _obj["y"] = this->_y;
    }
  };
  int getX(){ return this->_x; };
  int getY(){ return this->_y; };
};
ExecMeta* SCENARIO_EXECMETA_SAMPLE(){
  return execMeta<CalculateArgs>(EXEC_JSON_SOURCE);
};
bool EVAL_EXECMETA_SAMPLE(TestComparer op, ExecMeta* value, ExecMeta* target){
  if (op == TEST_EQUAL && value != NULL){
    if (value->getModule() != "execTester") return false;
    if (value->getMethod() != "calculate") return false;
    CalculateArgs* _args = value->getArgsAs<CalculateArgs>();
    if (_args == NULL) return false;
    if (_args->getX() != 20) return false;
    if (_args->getY() != 10) return false;
    return true;
  }
  else return false;
};

std::string SCENARIO_RESULTDATA_NATIVE_BOOL(){
  NativeData<bool>* _data = NativeData<bool>::create("bool", false);
  return _data->toJSON();
};
std::string SCENARIO_RESULTDATA_NATIVE_CHAR(){
  NativeData<char>* _data = NativeData<char>::create("char", 'A');
  return _data->toJSON();
};
std::string SCENARIO_RESULTDATA_NATIVE_INT(){
  NativeData<int>* _data = NativeData<int>::create("int", 100);
  return _data->toJSON();
};
std::string SCENARIO_RESULTDATA_NATIVE_CONSTCHAR(){
  NativeData<const char*>* _data = NativeData<const char*>::create("string", "Hello");
  return _data->toJSON();
};
std::string SCENARIO_RESULTDATA_NATIVE_STRING(){
  NativeData<std::string>* _data = NativeData<std::string>::create("string", "Hello");
  return _data->toJSON();
};

std::string SCENARIO_IRESULT_NATIVE_STRING(){
  return (new IResult("TEST_MOD", "test"))
    ->setData(NativeData<std::string>::create("string", "Hello World"))
    ->toJSON();
};
class SimpleObjectData: public IResultData{
private:
  std::string _val1;
  int _val2;
protected:
  SimpleObjectData(std::string val1, int val2): IResultData("object"){
    this->_val1 = val1;
    this->_val2 = val2;
  };
public:
  static SimpleObjectData* create(std::string val1, int val2){
    return new SimpleObjectData(val1, val2);
  };
  virtual void fillJSON(JsonVariant json){
    if (json.is<JsonObject>()){
      IResultData::fillJSON(json);
      JsonObject& _obj = json.as<JsonObject>();
      _obj["type"] = "SimpleObject";
      JsonObject& _data = _obj.createNestedObject("data");
      _data["val1"] = this->_val1.c_str();
      _data["val2"] = this->_val2;
    };
  };
  virtual std::string toJSON(){
    DynamicJsonBuffer _buffer(512);
    JsonObject& _obj = _buffer.createObject();
    this->fillJSON(_obj);
    return nodeJSON::stringify(_obj);
  };
};
std::string SCENARIO_IRESULT_OBJECT(){
  return (new IResult("TEST_MOD", "testObject"))
    ->setData(SimpleObjectData::create("Hello World", 100))
    ->toJSON();
};

class execTester: public IModule{
private:
  IResultData* calculate(IExecArgs* args){
    CalculateArgs* _args = args->as<CalculateArgs>();
    return nativeData<int>("int", (_args->getX() + _args->getY()));
  };
protected:
  virtual IModule::execHandler getHandler(std::string method){
    if (method == "calculate") return static_cast<IModule::execHandler>(&execTester::calculate);
    else return NULL;
  };
public:
  execTester(): IModule("execTester"){ };
};
IResult* SCENARIO_IMODULE_EXEC_ONEXECUTE(){
  return (new execTester())->execute(SCENARIO_EXECMETA_SAMPLE());
};
bool EVAL_IMODULE_EXEC_ONEXECUTE(TestComparer op, IResult* value, IResult* target){
  IResult* _comp = (new IResult("execTester", "calculate"))->setData(nativeData<int>("int", 30));
  return value->equal(_comp);
};
// IResult* SCENARIO_IMODULE_EXEC_NOMETHOD(){
//   return (new execTester())->execute("onNoMethod", "Hello World");
// };
// bool EVAL_IMODULE_EXEC_NOMETHOD(TestComparer op, IResult* value, IResult* target){
//   IResult* _comp = (new IResult("execTester", "onNoMethod"))->setData(exceptionData("MethodNotFound", "Method [onNoMethod] not found"));
//   return value->equal(_comp);
// };

void RUN_NODEBASE_TEST(){
  RUN_TEST<IResult*>("IOBJECT_NATIVE_EQUAL")
    ->scenario(&SCENARIO_IOBJECT_NATIVE_EQUAL)
    ->evalWith(TEST_EQUAL, &EVAL_IOBJECT_NATIVE_EQUAL)
    ->execute();
  RUN_TEST<std::string>("JSONSUPPORT_FROM_JSON")
    ->scenario(&SCENARIO_JSONSUPPORT_FROM_JSON)
    ->compareTo(TEST_EQUAL, "Hello")
    ->execute();
  RUN_TEST<std::string>("JSONSUPPORT_TO_JSON")
    ->scenario(&SCENARIO_JSONSUPPORT_TO_JSON)
    ->compareTo(TEST_EQUAL, JSON_SOURCE)
    ->execute();
  RUN_TEST<std::string>("JSONSUPPORT_FILL_OBJECT")
    ->scenario(&SCENARIO_JSONSUPPORT_FILL_OBJECT)
    ->compareTo(TEST_EQUAL, JSON_SOURCE)
    ->execute();
  RUN_TEST<std::string>("JSONSUPPORT_FILL_ARRAY")
    ->scenario(&SCENARIO_JSONSUPPORT_FILL_ARRAY)
    ->compareTo(TEST_EQUAL, "[" + JSON_SOURCE + "]")
    ->execute();

  RUN_TEST<ExecMeta*>("EXECMETA_SAMPLE")
    ->scenario(&SCENARIO_EXECMETA_SAMPLE)
    ->evalWith(TEST_EQUAL, &EVAL_EXECMETA_SAMPLE)
    ->execute();

  RUN_TEST<std::string>("RESULTDATA_NATIVE_BOOL")
    ->scenario(&SCENARIO_RESULTDATA_NATIVE_BOOL)
    ->compareTo(TEST_EQUAL, "{\"kind\":\"native\",\"type\":\"bool\",\"value\":false}")
    ->execute();
  RUN_TEST<std::string>("RESULTDATA_NATIVE_CHAR")
    ->scenario(&SCENARIO_RESULTDATA_NATIVE_CHAR)
    ->compareTo(TEST_EQUAL, "{\"kind\":\"native\",\"type\":\"char\",\"value\":65}")
    ->execute();
  RUN_TEST<std::string>("RESULTDATA_NATIVE_INT")
    ->scenario(&SCENARIO_RESULTDATA_NATIVE_INT)
    ->compareTo(TEST_EQUAL, "{\"kind\":\"native\",\"type\":\"int\",\"value\":100}")
    ->execute();
  RUN_TEST<std::string>("RESULTDATA_NATIVE_CONSTCHAR")
    ->scenario(&SCENARIO_RESULTDATA_NATIVE_CONSTCHAR)
    ->compareTo(TEST_EQUAL, "{\"kind\":\"native\",\"type\":\"string\",\"value\":\"Hello\"}")
    ->execute();
  RUN_TEST<std::string>("RESULTDATA_NATIVE_STRING")
    ->scenario(&SCENARIO_RESULTDATA_NATIVE_STRING)
    ->compareTo(TEST_EQUAL, "{\"kind\":\"native\",\"type\":\"string\",\"value\":\"Hello\"}")
    ->execute();

  RUN_TEST<std::string>("IRESULT_NATIVE_STRING")
    ->scenario(&SCENARIO_IRESULT_NATIVE_STRING)
    ->compareTo(TEST_EQUAL, "{\"TEST_MOD.test\":{\"kind\":\"native\",\"type\":\"string\",\"value\":\"Hello World\"}}")
    ->execute();
  RUN_TEST<std::string>("IRESULT_OBJECT")
    ->scenario(&SCENARIO_IRESULT_OBJECT)
    ->compareTo(TEST_EQUAL, "{\"TEST_MOD.testObject\":{\"kind\":\"object\",\"type\":\"SimpleObject\",\"data\":{\"val1\":\"Hello World\",\"val2\":100}}}")
    ->execute();

  RUN_TEST<IResult*>("IMODULE_EXEC_ONEXECUTE")
    ->scenario(&SCENARIO_IMODULE_EXEC_ONEXECUTE)
    ->evalWith(TEST_EQUAL, &EVAL_IMODULE_EXEC_ONEXECUTE)
    ->execute();
  // RUN_TEST<IResult*>("IMODULE_EXEC_NOMETHOD")
  //   ->scenario(&SCENARIO_IMODULE_EXEC_NOMETHOD)
  //   ->evalWith(TEST_EQUAL, &EVAL_IMODULE_EXEC_NOMETHOD)
  //   ->execute();
};