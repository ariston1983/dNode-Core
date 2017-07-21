#define __TEST_NAME_LENGTH__ 25
#define __TEST_LOG_STEPS__

#include "Arduino.h"
#include "dNode-Core.hpp"
#include "Test/test-List.hpp"
#include "dNode/node-Reflection.hpp"
// #include "dNode/node-Base.hpp"

using namespace dNode;


// class Base{
// protected:
//   virtual bool isType(const std::string&) const { return false; };
// public:
//   virtual const std::string& getTypeId() const = 0;
//   virtual bool equal(const Base&) const { return false; };
// };
// class Derived: public Base{
// protected:
//   virtual bool isType(const std::string& typeId) const override {
//     return (typeId == "Derived" || Base::isType(typeId));
//   };
// public:
//   virtual const std::string& getTypeId() const override {
//     static std::string _typeId("Derived");
//     return _typeId;
//   };
//   virtual bool equal(const Base& obj) const override {
//     return isType(obj.getTypeId());
//   };
// };
// class Child: public Base{
// protected:
//   virtual bool isType(const std::string& typeId) const override {
//     return (typeId == "Child" || Base::isType(typeId));
//   };
// public:
//   virtual const std::string& getTypeId() const override {
//     static std::string _typeId("Child");
//     return _typeId;
//   };
//   virtual bool equal(const Base& obj) const override {
//     return isType(obj.getTypeId());
//   };
// };
// bool isEqual(const Base& lhs, const Base& rhs){ return lhs.equal(rhs); };

void setup(){
  Serial.begin(115200);
  Serial.println();

  // Derived _d1;
  // Derived _d2;
  // Child _c1;
  // Child _c2;

  // Serial.println(isEqual(_d1, _d2));
  // Serial.println(isEqual(_d1, _c1));
  // Serial.println(isEqual(_c1, _c2));
};
void loop(){};
