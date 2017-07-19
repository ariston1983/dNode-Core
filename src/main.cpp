#define __TEST_NAME_LENGTH__ 25
#define __TEST_LOG_STEPS__

#include "Arduino.h"
#include "dNode-Core.hpp"
#include "Test/test-List.hpp"
// #include "dNode/node-Base.hpp"

using namespace dNode;

// class Base{
// public:
//   Base(){ };
//   virtual void test(){ };
// };
// class Derived: public Base{
// public:
//   Derived(): Base(){ };
//   virtual void test(){ };
// };

template<typename TDerived>
class Base{
private:
  TDerived* impl(){
    return static_cast<TDerived*>(this);
  };
public:
  typedef TDerived type;
  bool equal(Base<TDerived>* obj){
    return impl()->equal(static_cast<TDerived*>(obj));
  };
  template<typename TCompare>
  friend bool operator==(Base<TDerived>& lhs, TCompare& rhs){
    return ptr(lhs)->equal(ptr(rhs));
  };
};
class Derived: public Base<Derived>{
private:
  int _age;
public:
  Derived(int age){ this->_age = age; };
  int getAge(){ return this->_age; };
  bool equal(Derived* obj){
    return this->getAge() == obj->getAge();
  };
};
class Dump: public Base<Dump>{
};
template<typename T, typename U>
bool isTypeOf(Base<U>& value){
  return isSame<typename Base<U>::type, T>::value;
};

void setup(){
  Serial.begin(115200);
  Serial.println();

  Derived _a(10);
  Derived _b(10);
  //Serial.println(isEqual(_a, _b));
  Serial.println(isTypeOf<Derived>(_a));
  
  // Object* _f = new FirstClass("Mike");
  // Object* _s = new SecondClass(30);
  // Serial.println(val(_f) == val(_s));

  // TEST_INIT();
  // Test::Base::EXECUTE();
  // Test::Module::Base::EXECUTE();
};
void loop(){};
