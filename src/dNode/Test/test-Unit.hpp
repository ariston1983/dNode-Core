#pragma once

#include <string>
#include "../node-Helper.hpp"
#include "../Logger/node-Logger.hpp"

#ifndef __TEST_NAME_LENGTH__
#define __TEST_NAME_LENGTH__ 30
#endif

namespace dNode{
  namespace UnitTest{
    enum TEST_OP{
      TEST_EQUAL,
      TEST_NOT_EQUAL,
      TEST_GREATER_THAN,
      TEST_GREATER_OR_EQUAL,
      TEST_LESS_THAN,
      TEST_LESS_OR_EQUAL
    };

    template<typename T, typename U>
    struct testCompatible{
      static const bool value = 
        (isVariant<T>::value && (isVariant<U>::value || isNative<U>::value || isObject<U>::value)) ||
        (isSame<T, U>::value && (isObject<U>::value || isNative<U>::value));
    };
    template<typename T>
    using testGenerator = T(*)();
    template<typename T, typename U>
    using testEvaluator = bool(*)(TEST_OP, T, U);

    class ITestExecutor{
    public:
      virtual bool execute(){ return false; };
    };    

    template<typename TTest>
    class TestUnit{
      template<typename T, typename U>
      class TestExecutor: public ITestExecutor{
      private:
        std::string _testName;
        testGenerator<T> _scenario;
        TEST_OP _op;
        testEvaluator<T, U> _evaluator;
        U _target;
        std::string fixName(std::string name){
          std::string _name = "";
          if (name.length() > __TEST_NAME_LENGTH__) for (int _i = 0; _i < __TEST_NAME_LENGTH__; _i++) _name += name[_i];
          else _name = name;
          for (int _i = _name.length(); _i < __TEST_NAME_LENGTH__; _i++)
            _name += " ";
          return _name;
        };
      public:
        TestExecutor(std::string testName, testGenerator<T> scenario, TEST_OP op, testEvaluator<T, U> evaluator){
          this->_testName = fixName(testName);
          this->_scenario = scenario;
          this->_op = op;
          this->_evaluator = evaluator;
        };
        TestExecutor(std::string testName, testGenerator<T> scenario, TEST_OP op, testEvaluator<T, U> evaluator, U target){
          this->_testName = fixName(testName);
          this->_scenario = scenario;
          this->_op = op;
          this->_evaluator = evaluator;
          this->_target = target;
        };
        bool execute(){
          LOG("RUN  [" + this->_testName + "]");
          if (this->_scenario == NULL){
            LOG("TEST [" + this->_testName + "]", 1, "NO_SCENARIO");
            return false;
          };
          LOG("exec scenario", 2);
          T _eval = this->_scenario();
          LOG("scenario executed", 2);
          LOG("exec evaluator", 2);
          bool _test = this->_evaluator(this->_op, _eval, this->_target);
          return LOG<bool>("TEST [" + this->_testName + "]", _test, 1, _test ? "GOOD" : "BAD");
        };
      };
    private:
      std::string _testName;
      testGenerator<TTest> _scenario;
    private:
      template<typename TTarget>
      static bool objectEvaluator(TEST_OP op, TTest value, TTarget target){
        bool _res = false;
        LOG("compare", 2);
        switch (op){
          case TEST_NOT_EQUAL: _res = val(value) != val(target); break;
          case TEST_EQUAL:
          default: _res = val(value) == val(target); break;
        };
        return _res;
      };
      template<typename TTarget>
      static bool comparableEvaluator(TEST_OP op, TTest value, TTarget target){
        bool _res = false;
        LOG("compare", 2);
        switch (op){
          case TEST_NOT_EQUAL: _res = val(value) != val(target); break;
          case TEST_GREATER_THAN: _res = val(value) > val(target); break;
          case TEST_GREATER_OR_EQUAL: _res = val(value) >= val(target); break;
          case TEST_LESS_THAN: _res = val(value) < val(target); break;
          case TEST_LESS_OR_EQUAL: _res = val(value) <= val(target); break;
          case TEST_EQUAL:
          default: _res = val(value) == val(target); break;
        };
        return _res;
      };
    public:
      TestUnit(std::string testName, typename enableIf<testCompatible<TTest, TTest>::value, TTest>::type* = 0){
        this->_testName = testName;
        this->_scenario = NULL;
      };
      TestUnit<TTest>* scenario(testGenerator<TTest> func){
        this->_scenario = func;
        return this;
      };
      ITestExecutor* evalWith(TEST_OP op, testEvaluator<TTest, TTest> func){
        return new TestExecutor<TTest, TTest>(this->_testName, this->_scenario, op, func);
      };
      template<typename TTarget>
      typename enableIf<testCompatible<TTest, TTarget>::value && (isComparable<TTarget>::value || isNative<TTarget>::value), ITestExecutor>::type*
      evalWith(TEST_OP op, TTarget value){
        return new TestExecutor<TTest, TTarget>(this->_testName, this->_scenario, op, &TestUnit::comparableEvaluator, value);
      };
      template<typename TTarget>
      typename enableIf<testCompatible<TTest, TTarget>::value && (!isComparable<TTarget>::value && !isNative<TTarget>::value), ITestExecutor>::type*
      evalWith(TEST_OP op, TTarget value){
        return new TestExecutor<TTest, TTarget>(this->_testName, this->_scenario, op, &TestUnit::objectEvaluator, value);
      };
    };
  };
};