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

void setup(){
  Serial.begin(115200);
  Serial.println();

  // float _x = test<float>(10);

  InvokerArgument _arg("test", (int)100);
  int _x = (int)_arg;
  Serial.println(_x);

  // TEST_INIT();
  // RUN_NODEBASE_TEST();
};
void loop(){};
