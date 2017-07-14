#define __TEST_NAME_LENGTH__ 25
#define __TEST_LOG_STEPS__

#include "Arduino.h"
#include "dNode-Core.hpp"
#include "Test/test-List.hpp"
// #include "dNode/node-Base.hpp"

using namespace dNode;

class Base{
public:
  Base(){ };
};
class Derived: public Base{
public:
  Derived(): Base(){ };
};
class NonBase{
public:
  NonBase(){ };
};

// template<typename TBase, typename T>
// typename enableIf<isBaseOf<TBase, T>::value, bool>::type isValueOf(T* value){ return true; };
// template<typename TBase, typename T>
// typename enableIf<!isBaseOf<TBase, T>::value, bool>::type isValueOf(T* value){ return false; };

void setup(){
  Serial.begin(115200);
  Serial.println();

  Base* _x = new Base();
  Base* _y = new Derived();
  NonBase* _z = new NonBase();
  if (static_cast<Derived*>(_x)) Serial.println("Castable");
  else Serial.println("Uncastable");
  if (static_cast<Derived*>(_y)) Serial.println("Castable");
  else Serial.println("Uncastable");
  // if (static_cast<Derived*>(_z)) Serial.println("Castable");
  // else Serial.println("Uncastable");

  Serial.println(dNode::isValueOf<Derived>(_x));
  // Serial.println(dNode::isValueOf<Base>(new NonBase()));

  // TEST_INIT();
  // Test::Base::EXECUTE();
  // Test::Module::Base::EXECUTE();
};
void loop(){};
