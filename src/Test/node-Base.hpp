#pragma once

#include <string>
#include "../dNode/node-Base.hpp"
#include "../dNode/Test/node-Tester.hpp"

using namespace dNode;
using namespace dNode::UnitTest;

namespace Test{
  namespace Base{
    class MyObject: public dNode::Object{
    private:
      std::string _name;
    public:
      MyObject(std::string name): Object(){ this->_name = name; }
      std::string getName(){ return this->_name; };
      virtual bool equal(dNode::Object* obj){
        MyObject* _cmp = static_cast<MyObject*>(obj);
        return _cmp->getName() == this->_name;
      };
      virtual std::string toString(){ return this->getName(); };
    };
    Dictionary<std::string, Variant*>* createDictionary(){
      Dictionary<std::string, Variant*>* _dict = new Dictionary<std::string, Variant*>();
      _dict->add("name", dNode::var("Mike"));
      _dict->add("age", dNode::var(30));
      return _dict;
    };
    class MyDictionary: public Dictionary<std::string, Variant*>{
    public:
      MyDictionary(): Dictionary<std::string, Variant*>(){ };
      virtual bool equal(Object* obj){
        if (obj == NULL){ LOG("MyDictionary equality NULL", 2); return false; };
        if (!dNode::isValueOf<MyDictionary>(obj)){ LOG("MyDictionary equality invalid value", 2); return false; };
        LOG("MyDictionary equality execute Dictionary equality", 2);
        // Dictionary<std::string, Variant*>* _cmp = static_cast<Dictionary<std::string, Variant*>*>(obj);
        return Dictionary<std::string, Variant*>::equal(static_cast<Dictionary<std::string, Variant*>*>(obj));
      };
    };
    MyDictionary* createMyDictionary(){
      MyDictionary* _dict = new MyDictionary();
      _dict->add("name", dNode::var("Mike"));
      _dict->add("age", dNode::var(30));
      return _dict;
    };
    dNode::Variant* createVariantObject(){
      return dNode::var(new MyObject("Mike"));
    };

    void EXECUTE(){
      RUN_TEST<int>("TESTING_TEST_EQ")
      ->scenario([](){ return 10; })
      ->evalWith(TEST_EQUAL, 10)
      ->execute();
      RUN_TEST<int>("TESTING_TEST_NE")
      ->scenario([](){ return 10; })
      ->evalWith(TEST_NOT_EQUAL, 20)
      ->execute();
      RUN_TEST<int>("TESTING_TEST_GT")
      ->scenario([](){ return 10; })
      ->evalWith(TEST_GREATER_THAN, 5)
      ->execute();
      RUN_TEST<int>("TESTING_TEST_GTE")
      ->scenario([](){ return 10; })
      ->evalWith(TEST_GREATER_OR_EQUAL, 10)
      ->execute();
      RUN_TEST<int>("TESTING_TEST_LT")
      ->scenario([](){ return 10; })
      ->evalWith(TEST_LESS_THAN, 20)
      ->execute();
      RUN_TEST<int>("TESTING_TEST_LTE")
      ->scenario([](){ return 5; })
      ->evalWith(TEST_LESS_OR_EQUAL, 5)
      ->execute();

      RUN_TEST<int>("TESTING_TEST_EVALUATOR")
      ->scenario([](){ return 10; })
      ->evalWith([](TEST_OP op, int value){
        switch (op){
          case TEST_NOT_EQUAL: return value != 10;
          case TEST_GREATER_THAN: return value > 10;
          case TEST_GREATER_OR_EQUAL: return value >= 10;
          case TEST_LESS_THAN: return value < 10;
          case TEST_LESS_OR_EQUAL: return value <= 10;
          case TEST_EQUAL:
          default: return value == 10;
        };
      })
      ->execute();

      RUN_TEST<bool>("TESTING_VALUEOF_OBJECT")
      ->scenario([](){ return dNode::isValueOf<dNode::Object>(new MyObject("Mike")); })
      ->evalWith(TEST_EQUAL, true)
      ->execute();
      RUN_TEST<bool>("TESTING_VALUEOF_MYOBJECT")
      ->scenario([](){ return dNode::isValueOf<MyObject>(new dNode::Object()); })
      ->evalWith(TEST_EQUAL, true)
      ->execute();

      RUN_TEST<int>("TESTING_INT_CASTING")
      ->scenario([](){ return val(ptr(100)); })
      ->evalWith(TEST_EQUAL, 100)
      ->execute();
      RUN_TEST<float>("TESTING_FLOAT_CASTING")
      ->scenario([](){ return val(ptr(1.5f)); })
      ->evalWith(TEST_EQUAL, 1.5f)
      ->execute();
      RUN_TEST<const char*>("TESTING_CHARS_CASTING")
      ->scenario([](){ return val(ptr("Hello")); })
      ->evalWith(TEST_EQUAL, "Hello")
      ->execute();

      RUN_TEST<MyObject*>("TEST_OBJECT_EQUAL")
      ->scenario([](){ return new MyObject("Mike"); })
      ->evalWith(TEST_EQUAL, new MyObject("Mike"))
      ->execute();
      RUN_TEST<MyObject*>("TEST_OBJECT_NOTEQUAL")
      ->scenario([](){ return new MyObject("Mike"); })
      ->evalWith(TEST_NOT_EQUAL, new MyObject("Sarah"))
      ->execute();

      RUN_TEST<Dictionary<std::string, dNode::Variant*>*>("TEST_DICTIONARY_EQ")
      ->scenario([](){ return createDictionary(); })
      ->evalWith([](TEST_OP op, Dictionary<std::string, dNode::Variant*>* value){
        if (value == NULL){ LOG("missing value", 2); return false; }
        if (val(value) != val(createDictionary())){ LOG("invalid dictionary", 2); return false; }
        return true;
      })
      ->execute();
      RUN_TEST<MyDictionary*>("TEST_MYDICTIONARY_EQ")
      ->scenario([](){ return createMyDictionary(); })
      ->evalWith([](TEST_OP op, MyDictionary* value){
        if (value == NULL){ LOG("missing value", 2); return false; }
        if (val(value) != val(createMyDictionary())){ LOG("invalid dictionary", 2); return false; }
        return true;
      })
      ->execute();
      RUN_TEST<Variant*>("TEST_VAR_MYDICTIONARY")
      ->scenario([](){ return dNode::var(createMyDictionary()); })
      ->evalWith([](TEST_OP op, Variant* value){
        if (value == NULL){ LOG("missing value", 2); return false; }
        if (val(value) != val(dNode::var(createMyDictionary()))){ LOG("invalid dictionary", 2); return false; }
        return true;
      })
      ->execute();

      RUN_TEST<dNode::Variant*>("TEST_VARIANT_BOOL_EQ")
      ->scenario([](){ return dNode::var(true); })
      ->evalWith(TEST_EQUAL, true)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_BOOL_NE")
      ->scenario([](){ return dNode::var(true); })
      ->evalWith(TEST_NOT_EQUAL, false)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_INT_EQ")
      ->scenario([](){ return dNode::var(1); })
      ->evalWith(TEST_EQUAL, 1)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_INT_NE")
      ->scenario([](){ return dNode::var(1); })
      ->evalWith(TEST_NOT_EQUAL, 2)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_INT_GT")
      ->scenario([](){ return dNode::var(2); })
      ->evalWith(TEST_GREATER_THAN, 1)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_INT_GTE")
      ->scenario([](){ return dNode::var(2); })
      ->evalWith(TEST_GREATER_OR_EQUAL, 2)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_INT_LT")
      ->scenario([](){ return dNode::var(1); })
      ->evalWith(TEST_LESS_THAN, 2)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_INT_LTE")
      ->scenario([](){ return dNode::var(1); })
      ->evalWith(TEST_LESS_OR_EQUAL, 1)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_FLOAT_EQ")
      ->scenario([](){ return dNode::var(1.5f); })
      ->evalWith(TEST_EQUAL, 1.5f)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_FLOAT_NE")
      ->scenario([](){ return dNode::var(1.5f); })
      ->evalWith(TEST_NOT_EQUAL, 2.5f)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_FLOAT_GT")
      ->scenario([](){ return dNode::var(2.5f); })
      ->evalWith(TEST_GREATER_THAN, 1.5f)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_FLOAT_GTE")
      ->scenario([](){ return dNode::var(2.5f); })
      ->evalWith(TEST_GREATER_OR_EQUAL, 2.5f)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_FLOAT_LT")
      ->scenario([](){ return dNode::var(1.5f); })
      ->evalWith(TEST_LESS_THAN, 2.5f)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_FLOAT_LTE")
      ->scenario([](){ return dNode::var(1.5f); })
      ->evalWith(TEST_LESS_OR_EQUAL, 1.5f)
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_CHARS_EQ")
      ->scenario([](){ return dNode::var("Hello"); })
      ->evalWith(TEST_EQUAL, "Hello")
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_CHARS_NE")
      ->scenario([](){ return dNode::var("Hello"); })
      ->evalWith(TEST_NOT_EQUAL, "World")
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_CHARS_GT")
      ->scenario([](){ return dNode::var("World"); })
      ->evalWith(TEST_GREATER_THAN, "Hello")
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_CHARS_GTE")
      ->scenario([](){ return dNode::var("World"); })
      ->evalWith(TEST_GREATER_OR_EQUAL, "World")
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_CHARS_LT")
      ->scenario([](){ return dNode::var("Hello"); })
      ->evalWith(TEST_LESS_THAN, "World")
      ->execute();
      RUN_TEST<dNode::Variant*>("TEST_VARIANT_CHARS_LTE")
      ->scenario([](){ return dNode::var("Hello"); })
      ->evalWith(TEST_LESS_OR_EQUAL, "Hello")
      ->execute();

      RUN_TEST<dNode::Variant*>("TEST_VARIANT_OBJECT_EQ")
      ->scenario([](){ return createVariantObject(); })
      ->evalWith([](TEST_OP op, dNode::Variant* value){
        if (value == NULL){ LOG("missing value", 2); };
        if (val(value) != val(createVariantObject())){ LOG("invalid value", 2); };
        return true;
      })
      ->execute();
    };
  };
};