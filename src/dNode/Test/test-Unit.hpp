#pragma once

#include <string>
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
private:
  std::string _testname;
  function _scenario;
  TestComparer _comparer;
  function _with;
  TTestValue _target;
public:
  testUnit(std::string testname){
    this->_testname = testname;
    this->_scenario = NULL;
    this->_with = NULL;
  };
  testUnit<TTestValue>* scenario(function func){
    this->_scenario = func;
    return this;
  };
  testUnit<TTestValue>* checkIf(TestComparer op){
    this->_comparer = op;
    return this;
  };
  testUnit<TTestValue>* with(function func){
    this->_with = func;
    return this;
  };
  testUnit<TTestValue>* to(TTestValue value){
    this->_target = value;
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
    LOG("define target", 2);
    TTestValue _result = this->_target;
    if (this->_with != NULL){
      LOG("exec compare function", 2);
      _result = this->_with();
      LOG("compare function executed", 2);
    }
    LOG("target defined", 2);
    bool _test = false;
    LOG("compare", 2);
    switch (this->_comparer){
      case TEST_NOT_EQUAL: _test = _eval != _result; break;
      case TEST_GREATER_THAN: _test = _eval > _result; break;
      case TEST_GREATER_OR_EQUAL: _test = _eval >= _result; break;
      case TEST_LESS_THAN: _test = _eval < _result; break;
      case TEST_LESS_OR_EQUAL: _test = _eval <= _result; break;
      case TEST_EQUAL:
      default: _test = _eval == _result; break;
    };
    return LOG<bool>("TEST [" + this->_testname + "]", _test, 1, _test ? "GOOD" : "BAD");
  };
};