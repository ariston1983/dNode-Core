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

// typedef std::map<std::string, InvokerArgument> argss_Type;
// argss_Type* _args;
// argss_Type* getArgs(){
//   if (_args == NULL) _args = new argss_Type();
//   return _args;
// };

// class TestObject: public IJSONSupport{
// public:
//   virtual void fillJSON(JsonVariant json){
//     if (json.is<JsonObject>()){
//       json["name"] = "dodol garut";
//       json["age"] = 10;
//     }
//   };
// };

// template<typename T>
// void printJSON(T* value, typename enableIf<isBaseOf<IJSONSupport, T>::value>::type* = 0){
//   IJSONSupport* _obj = static_cast<IJSONSupport*>(value);
//   Serial.println(_obj->toJSON().c_str());
// };

// class TestObject: public dNode::Object{
// private:
//   std::string _name;
// public:
//   TestObject(std::string name): Object(){ this->_name = name; };
//   virtual std::string toString(){ return this->_name; };
// };
// class MyList: public dNode::List<TestObject*>{ };
// void foreachTestObject(dNode::Object* context, TestObject* item){
//   Serial.println(item->toString().c_str());
// };
// void foreachInt(dNode::Object* context, int item){
//   Serial.println(item);
// };
// void foreachDict(dNode::Object* context, std::string key, TestObject* value){
//   Serial.println(key.c_str());
//   Serial.print("..."); Serial.println(value->toString().c_str());
// };
// template<typename T>
// void checkReference(){
//   Serial.println(isBaseOf<dNode::Object, typename clearReference<T>::type>::value && isReference<T>::value);
//   Serial.println(isSame<typename enableIf<isBaseOf<dNode::Object,
//    typename clearReference<T>::type>::value && isReference<T>::value, typename clearReference<T>::type>::type,
//    typename clearReference<T>::type>::value);
// };
// template<typename T>
// bool printReference(T& value, typename enableIf<isBaseOf<dNode::Object,
//   typename clearReference<T>::type>::value && isReference<T>::value, typename clearReference<T>::type>::type* = 0){
//   Serial.println(value.toString().c_str());
// };

// template<class T>
// void TestObject(T value, typename enableIf<isObject<T>::value>::type* = 0){
//   MyObject* _obj = static_cast<MyObject*>(value);
//   _obj->print();
// };

class TestObject: public dNode::Object{
private:
  std::string _name;
public:
  TestObject(std::string name){ this->_name = name; };
  std::string getName(){ return this->_name ; };
  virtual bool equal(dNode::Object* obj){
    Serial.println("TestObject compare");
    TestObject* _cmp = static_cast<TestObject*>(obj);
    Serial.println(this->getName().c_str());
    Serial.println(_cmp->getName().c_str());
    return this->_name == _cmp->getName();
  };
  virtual std::string toString(){
    return this->getName();
  };
};
// void iterate(dNode::Object* obj){
//   dNode::List<dNode::Variant*>* _cmp = static_cast<dNode::List<dNode::Variant*>*>(obj);
//   dNode::Variant* _var = (*_cmp)[0];
//   TestObject* _to = _var->as<TestObject*>();
//   Serial.println(_to->getName().c_str());
// };
// bool compare(dNode::Object* o1, dNode::Object* o2){
//   dNode::Variant* _v1 = static_cast<dNode::Variant*>(o1);
//   dNode::Variant* _v2 = static_cast<dNode::Variant*>(o2);
//   return cast_value(_v1) == cast_value(_v2);
// };


void setup(){
  Serial.begin(115200);
  Serial.println();

  // TestObject* _o1 = new TestObject("Mike");
  // TestObject* _o2 = new TestObject("Mike");
  // dNode::Variant* _v1 = var(_o1);
  // dNode::Variant* _v2 = var(_o2);
  // Serial.println(val(_v1) == val(_v2));
  
  // dNode::List<dNode::Variant*>* _lv1 = new dNode::List<dNode::Variant*>();
  // _lv1->add(var(new TestObject("Mike")));
  // _lv1->add(var(new TestObject("Sarah")));
  // dNode::List<dNode::Variant*>* _lv2 = new dNode::List<dNode::Variant*>();
  // _lv2->add(var(new TestObject("Mike")));
  // _lv2->add(var(new TestObject("Sarah")));
  // Serial.println(val(_lv1) == val(_lv2));

  dNode::Dictionary<std::string, dNode::Variant*>* _dv1 = new dNode::Dictionary<std::string, dNode::Variant*>();
  _dv1->add("Name", dNode::var("Mike"));
  _dv1->add("Age", dNode::var(30));
  _dv1->add("Object", var(new TestObject("Mike")));
  dNode::Dictionary<std::string, dNode::Variant*>* _dv2 = new dNode::Dictionary<std::string, dNode::Variant*>();
  _dv2->add("Name", dNode::var("Mike"));
  _dv2->add("Age", dNode::var(30));
  _dv2->add("Object", var(new TestObject("Mike")));
  Serial.println(val(_dv1) == val(_dv2));

  // iterate(_lv2);
  // Serial.println(compare((*_lv1)[0], (*_lv2)[0]));
  // Serial.println(cast_value((*_lv1)[0]).as<TestObject*>()->getName().c_str());
  // Serial.println(cast_value((*_lv2)[0]).as<TestObject*>()->getName().c_str());
  // Serial.println(cast_value((*_lv1)[0]).equal(cast_value((*_lv2)[0])));

  // dNode::List<int>* _l1 = new dNode::List<int>();
  // _l1->add(100);
  // _l1->add(200);
  // dNode::List<int>* _l2 = new dNode::List<int>();
  // _l2->add(200);
  // _l2->add(300);
  // Serial.println(cast_value(_l1) == cast_value(_l2));

  // Serial.println(cast_value(_v1) == cast_value(_v2));

  // std::string _st1 = "Hello";
  // std::string _st2 = "Hello";
  // Serial.println(cast_value(_st1) == cast_value(_st2));
  // std::string _st3 = "World";
  // Serial.println(cast_value(_st1) == cast_value(_st3));

  // enable<const char*>();

  // TestObject _t("Mike");
  // checkReference<TestObject&>();
  // printReference<TestObject&>(_t);
  // dNode::Variant _var = var(new TestObject("Mike"));
  // TestObject* _obj = (TestObject*)_var; //_var->as<TestObject*>();
  // Serial.println(_obj->toString().c_str());

  // dNode::Variant* _var = new dNode::Variant("Hello");
  // Serial.println(_var->as<const char*>());

  // MyList* _list = new MyList();
  // _list->add(new TestObject("Mike"));
  // _list->add(new TestObject("Sarah"));
  // _list->add(new TestObject("Rachel"));
  // _list->forEach(&foreachTestObject);

  // dNode::List<int>* _iList = new dNode::List<int>();
  // _iList->add(100);
  // _iList->add(200);
  // _iList->add(300);
  // _iList->forEach(&foreachInt);

  // dNode::Dictionary<std::string, TestObject*>* _dict = new dNode::Dictionary<std::string, TestObject*>();
  // _dict->add("one", new TestObject("Mike"));
  // _dict->add("two", new TestObject("Sarah"));
  // _dict->add("three", new TestObject("Rachel"));
  // _dict->forEach(&foreachDict);

  // TestObject* _obj = new TestObject();
  // printJSON(_obj);

  // float _x = test<float>(10);

  // InvokerArgument _arg("test", (int)100);
  // Serial.println((int)_arg);
  // _arg = (int)200;
  // Serial.println((int)_arg);

  // std::map<std::string, InvokerArgument>* _args = new std::map<std::string, InvokerArgument>();
  // Serial.println(getArgs()->empty());

  // Invoker* _iv = new Invoker("modTest", "hello");
  // _iv->add("name", "John Doe").add("age", 30);
  // std::string _name = _iv["name"];
  // int _age = _iv["age"];
  // Serial.println(_name.c_str());
  // Serial.println(_age);
  // for (auto _it: *_iv->getParamNames())
  //   Serial.println(_it.c_str());
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
