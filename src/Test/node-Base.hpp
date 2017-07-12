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

  RUN_TEST<dNode::Variant*>("TEST_VARIANT_BOOL")
  ->scenario([](){ return dNode::var(true); })
  ->evalWith<bool>(TEST_EQUAL, true)
  ->execute();
};