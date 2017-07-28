#define __TEST_NAME_LENGTH__ 25
#define __TEST_LOG_STEPS__

#include "Arduino.h"
// #include "dNode-Core.hpp"
// #include "Test/test-List.hpp"
#include "dNode/node-Helper.hpp"
#include "dNode/node-Base.hpp"
#include "dNode/node-Reflection.hpp"
// #include "dNode/node-Base.hpp"

using namespace dNode;
using namespace dNode.Reflection;

class Base: public Object{
OBJECT_TYPEINFO("Base")
REFLECTION_TYPEINFO(Base, dNode::Object)
};
class Derived: public Base{
OBJECT_TYPEINFO("Derived")
REFLECTION_TYPEINFO(Derived, Base)
};
class Child: public Base{
OBJECT_TYPEINFO("Child")
REFLECTION_TYPEINFO(Child, Base)
};

// template<typename T>
// std::string addOn(){
//   return T::getTypeId();
// };

// class Base{
// public:
//   static const std::string& getTypeId(){
//     static std::string _typeId("Base");
//     return _typeId;
//   };
//   virtual bool assignableFrom(Base& obj){
//     Serial.print("This class: "); Serial.println(getType().c_str());
//     Serial.print("That class: "); Serial.println(obj.getType().c_str());
//     return obj.isSubclassOf(this->getType());
//   };
//   virtual const std::string& getType() const { return Base::getTypeId(); };
//   virtual bool isSubclassOf(std::string typeId){ return typeId == getType(); };
// };
// template<typename T, typename U>
// class Derived: public Base{
// public:
//   static const std::string& getTypeId(){
//     static std::string _typeId("Derived<");
//     if (_typeId == "Derived<"){
//       _typeId += addOn<T>();
//       _typeId += ",";
//       _typeId += addOn<U>();
//       _typeId += ">";
//     };
//     return _typeId;
//   };
//   virtual const std::string& getType() const override { return Derived::getTypeId(); };
//   virtual bool isSubclassOf(std::string typeId) override {
//     Serial.print("This type: "); Serial.println(getType().c_str());
//     Serial.print("That type: "); Serial.println(typeId.c_str());
//     return typeId == getType();
//   };
//   bool equal(Base& obj){
//     Serial.println(obj.getType().c_str());
//     return assignableFrom(obj);
//   };
// };
// class Child: public Base{
// public:
//   static const std::string& getTypeId(){
//     static std::string _typeId("Child");
//     return _typeId;
//   };
//   virtual const std::string& getType() const override { return Child::getTypeId(); };
// };
// class Dummy: public Base{
// public:
//   static const std::string& getTypeId(){
//     static std::string _typeId("Dummy");
//     return _typeId;
//   };
//   virtual const std::string& getType() const override { return Dummy::getTypeId(); };
// };
// class DerivedChild: public Derived<Child, Dummy>{
// public:
//   static const std::string& getTypeId(){
//     static std::string _typeId("DerivedChild");
//     return _typeId;
//   };
//   virtual const std::string& getType() const override { return DerivedChild::getTypeId(); };
//   virtual bool isSubclassOf(std::string typeId) override { return typeId == getType() || Derived<Child, Dummy>::isSubclassOf(typeId); };
// };

// template<typename T>
// void printTypeId(){
//   Serial.println(T::getTypeId().c_str());
// };

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

  // printTypeId<Base>();
  // printTypeId<Child>();
  // printTypeId<Dummy>();
  // printTypeId<Derived<Child, Dummy>>();

  // Derived<Child, Dummy> _d1;
  // Derived<Child, Dummy> _d2;
  // Derived<Child, Child> _d3;

  // Serial.println(_d1.equal(&_d2));
  // Serial.println(_d1.equal(&_d3));

  // Derived<Child, Dummy> _d1;
  // Derived<Dummy, Child> _d2;
  // DerivedChild _dc;
  // Serial.println(_d1.getType().c_str());
  // Serial.println(_d2.getType().c_str());
  // Serial.println(_dc.getType().c_str());
  // Serial.println();
  // Serial.println(_d1.equal(_dc));
  // Serial.println(_dc.equal(_d1));
  
  // Derived _d1;
  // Derived _d2;
  // Child _c1;
  // Child _c2;

  // Serial.println(isEqual(_d1, _d2));
  // Serial.println(isEqual(_d1, _c1));
  // Serial.println(isEqual(_c1, _c2));
};
void loop(){};
