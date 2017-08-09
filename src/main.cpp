#define __TEST_NAME_LENGTH__ 25
#define __TEST_LOG_STEPS__

#include "Arduino.h"
#include <string>
// #include "dNode-Core.hpp"
// #include "Test/test-List.hpp"
#include "dNode/node-Helper.hpp"
#include "dNode/node-Base.hpp"
// #include "dNode/node-Reflection.hpp"
// #include "dNode/node-Base.hpp"

class Child1: public dNode::Object{
CLASS_INFO(Child1, dNode::Object, "child1")
private:
  int _value;
public:
  int setValue(int val){ _value = val; };
  int getValue(){ return _value; };
  virtual bool equal(Object& obj) override{
    if (!obj.isTypeOf("child1")) return false;
    Child1 _obj = *static_cast<Child1*>(&obj);
    return getValue() == _obj.getValue();
  };
};
class Child11: public Child1{
CLASS_INFO(Child11, Child1, "child11")
public:
  virtual bool equal(Object& obj) override{ return false; };
};
class Child2: public dNode::Object{
CLASS_INFO(Child2, dNode::Object, "child2")
public:
  virtual bool equal(Object& obj) override{ return false; };
};

bool exec(dNode::Object& params){
  if (!params.isTypeOf("child1")) return false;
  Child1 _obj = *static_cast<Child1*>(&params);
  Serial.println(_obj.getValue());
  return true;
};

void setup(){
  Serial.begin(115200);
  Serial.println();

  Child1 _ch1;
  _ch1.setValue(10);
  Child11 _ch11;
  _ch11.setValue(10);
  Child11 _ch11_1;
  _ch11_1.setValue(100);
  Child2 _ch2;
  Serial.println(_ch11.isTypeOf("child1"));
  Serial.println(_ch11.isTypeOf("dnode::object"));
  Serial.println(_ch2.isTypeOf("child1"));
  Serial.println(_ch2.isTypeOf("dnode::object"));
  Serial.println(_ch1.equal(_ch11));
  Serial.println(_ch1.equal(_ch11_1));
  Serial.println(exec(_ch1));
  Serial.println(exec(_ch11));
  Serial.println(exec(_ch11_1));
  Serial.println(exec(_ch2));
};
void loop(){};
