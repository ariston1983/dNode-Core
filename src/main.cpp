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

template<typename T>
class Base{
private:
  const T* impl() const{
    return static_cast<const T*>(this);
  };
public:
  template<typename U>
  bool is() const{
    return impl()->template as<U>();
  };
};
class Derived: public Base<Derived>{
public:
  template<typename U>
  bool is() const{
    return isNative<U>::value;
  };
};

void setup(){
  Serial.begin(115200);
  Serial.println();
  
  Derived* _d = new Derived();
  Serial.println(_d->is<bool>());
  Serial.println(_d->is<Derived>());

  // Object* _f = new FirstClass("Mike");
  // Object* _s = new SecondClass(30);
  // Serial.println(val(_f) == val(_s));

  // TEST_INIT();
  // Test::Base::EXECUTE();
  // Test::Module::Base::EXECUTE();
};
void loop(){};
