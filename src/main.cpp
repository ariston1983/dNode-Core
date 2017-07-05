#include "Arduino.h"
#include "dNode-Core.hpp"
// #include "Test/test-List.hpp"

// template<bool condition, typename T = void>
// struct enableIf{ };
// template<typename T>
// struct enableIf<true, T>{
//   typedef T type;
// };

// template<typename T, typename U>
// struct isSame{ static const bool value = false; };
// template<typename T>
// struct isSame<T, T>{ static const bool value = true; };
// template<typename T>
// struct isInt{ static const bool value = isSame<int, T>::value; };

// template<typename T>
// T test(T value, typename enableIf<isInt<T>::value>::type* = 0){
//   return value;
// }

// union TestUnion{
//   int asInt;
//   float asFloat;
// };

typedef std::map<std::string, InvokerArgument> argss_Type;
argss_Type* _args;
argss_Type* getArgs(){
  if (_args == NULL) _args = new argss_Type();
  return _args;
};

void setup(){
  Serial.begin(115200);
  Serial.println();

  // float _x = test<float>(10);

  // InvokerArgument _arg("test", (int)100);
  // Serial.println((int)_arg);
  // _arg = (int)200;
  // Serial.println((int)_arg);

  // std::map<std::string, InvokerArgument>* _args = new std::map<std::string, InvokerArgument>();
  // Serial.println(getArgs()->empty());

  Invoker* _iv = new Invoker("modTest", "hello");
  _iv->add("name", "John Doe").add("age", 30);
  // std::string _name = _iv["name"];
  // int _age = _iv["age"];
  // Serial.println(_name.c_str());
  // Serial.println(_age);
  for (auto _it: *_iv->getParamNames())
    Serial.println(_it.c_str());
  // TestUnion _a;
  // _a.asInt = 100;
  // Serial.println(_a.asInt);
  // _a.asFloat = 1.5;
  // Serial.println(_a.asFloat);
  // Serial.println(_a.asInt);

  // TEST_INIT();
  // RUN_NODEBASE_TEST();
};
void loop(){};
