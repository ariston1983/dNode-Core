#pragma once

#include <string>
#include "../node-Helper.hpp"
#include "../Logger/node-Logger.hpp"

enum TestComparer{
  TEST_EQUAL,
  TEST_NOT_EQUAL,
  TEST_GREATER_THAN,
  TEST_GREATER_OR_EQUAL,
  TEST_LESS_THAN,
  TEST_LESS_OR_EQUAL
};

template<typename TTestValue>
class testUnit{
  typedef TTestValue(*function)();
  typedef bool(*eval)(TestComparer, TTestValue, TTestValue);
private:
  std::string _testname;
  function _scenario;
  TestComparer _op;
  eval _evaluator;
  TTestValue _target;
private:
  static bool basicEvaluator(TestComparer op, TTestValue value, TTestValue target){
    bool _test = false;
    LOG("compare", 2);
    switch (op){
      case TEST_NOT_EQUAL: _test = value != target; break;
      case TEST_GREATER_THAN: _test = value > target; break;
      case TEST_GREATER_OR_EQUAL: _test = value >= target; break;
      case TEST_LESS_THAN: _test = value < target; break;
      case TEST_LESS_OR_EQUAL: _test = value <= target; break;
      case TEST_EQUAL:
      default: _test = value == target; break;
    };
  };
public:
  testUnit(std::string testname){
    this->_testname = testname;
    this->_scenario = NULL;
    this->_evaluator = NULL;
  };
  testUnit<TTestValue>* scenario(function func){
    this->_scenario = func;
    return this;
  };
  testUnit<TTestValue>* evalWith(TestComparer op, eval func){
    this->_op = op;
    this->_evaluator = func;
    return this;
  };
  testUnit<TTestValue>* compareTo(TestComparer op, TTestValue value){
    this->_target = value;
    this->_op = op;
    this->_evaluator = &testUnit::basicEvaluator;
    return this;
  };
  bool execute(){
    LOG("RUN [" + this->_testname + "]");
    if (this->_scenario == NULL){
      LOG("TEST [" + this->_testname + "]", 1, "NO_SCENARIO");
      return false;
    };
    LOG("exec scenario", 2);
    TTestValue _eval = this->_scenario();
    LOG("scenario executed", 2);
    LOG("exec evaluator", 2);
    bool _test = this->_evaluator(this->_op, _eval, this->_target);
    return LOG<bool>("TEST [" + this->_testname + "]", _test, 1, _test ? "GOOD" : "BAD");
  };
};