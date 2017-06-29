#include "Arduino.h"
#include <string>
#include <vector>
#include <map>
#include "dNode/node-Helper.hpp"
#include "dNode/node-Base.hpp"

class modTest: public IModule{
public:
  modTest(): IModule("modTest", "1.0"){ };
  virtual listRemoteMethod_Type* getRemoteMethods(){
    listRemoteMethod_Type* _methods = new listRemoteMethod_Type();
    _methods->push_back(createRemoteMethod("modTest", "test1")->addParam("param1", "string"));
    _methods->push_back(createRemoteMethod("modTest", "test2")->addParam("param1", "int"));//->addParam("param2", "string"));
    _methods->push_back(createRemoteMethod("modTest", "test3")->addParam("param1", "float"));//->addParam("param2", "int")->addParam("param3", "bool"));
    return _methods;
  };
};

void setup(){
  Serial.begin(115200);
  Serial.println();

  std::string _json = createRemoteMethod("test", "hello")->addParam("name", "string")->toJSON();
  Serial.println(_json.c_str());

  modTest* _test = new modTest();
  std::vector<RemoteMethod*>* _methods = _test->getRemoteMethods();

  DynamicJsonBuffer _buffer(512);
  JsonArray& _arr = _buffer.createArray();
  for (std::vector<RemoteMethod*>::iterator _method = _methods->begin(); _method != _methods->end(); ++_method){
    //Serial.println((*_method)->toJSON().c_str());
    (*_method)->fillJSON(_arr);
  };
  Serial.println(nodeJSON::stringify(_arr).c_str());
};
void loop(){};
