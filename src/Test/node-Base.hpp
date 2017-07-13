#pragma once

#include <string>
#include "../dNode/node-Base.hpp"
#include "../dNode/Test/node-Tester.hpp"

using namespace dNode;
using namespace dNode::UnitTest;

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

void RUN_NODEBASE_TEST(){
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

  RUN_TEST<MyObject*>("TEST_OBJECT_EQUAL")
  ->scenario([](){ return new MyObject("Mike"); })
  ->evalWith(TEST_EQUAL, new MyObject("Mike"))
  ->execute();
  RUN_TEST<MyObject*>("TEST_OBJECT_NOTEQUAL")
  ->scenario([](){ return new MyObject("Mike"); })
  ->evalWith(TEST_NOT_EQUAL, new MyObject("Sarah"))
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
  ->scenario([](){ return dNode::var(float(1.5)); })
  ->evalWith(TEST_EQUAL, float(1.5))
  ->execute();
  RUN_TEST<dNode::Variant*>("TEST_VARIANT_FLOAT_NE")
  ->scenario([](){ return dNode::var(float(1.5)); })
  ->evalWith(TEST_NOT_EQUAL, float(2.5))
  ->execute();
  RUN_TEST<dNode::Variant*>("TEST_VARIANT_FLOAT_GT")
  ->scenario([](){ return dNode::var(float(2.5)); })
  ->evalWith(TEST_GREATER_THAN, float(1.5))
  ->execute();
  RUN_TEST<dNode::Variant*>("TEST_VARIANT_FLOAT_GTE")
  ->scenario([](){ return dNode::var(float(2.5)); })
  ->evalWith(TEST_GREATER_OR_EQUAL, float(2.5))
  ->execute();
  RUN_TEST<dNode::Variant*>("TEST_VARIANT_FLOAT_LT")
  ->scenario([](){ return dNode::var(float(1.5)); })
  ->evalWith(TEST_LESS_THAN, float(2.5))
  ->execute();
  RUN_TEST<dNode::Variant*>("TEST_VARIANT_FLOAT_LTE")
  ->scenario([](){ return dNode::var(float(1.5)); })
  ->evalWith(TEST_LESS_OR_EQUAL, float(1.5))
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
};