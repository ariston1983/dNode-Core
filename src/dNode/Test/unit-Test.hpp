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
    template<typename T>
    using testEvaluator = bool(*)(TEST_OP, T);
    template<typename T, typename U>
    using targetTestEvaluator = bool(*)(TEST_OP, T, U);

    class ITestExecutor{
    protected:
      std::string _name;
      static std::string fixName(std::string name){
        std::string _name = "";
        if (name.length() > __TEST_NAME_LENGTH__) for (int _i = 0; _i < __TEST_NAME_LENGTH__; _i++) _name += name[_i];
        else _name = name;
        for (int _i = _name.length(); _i < __TEST_NAME_LENGTH__; _i++)
          _name += " ";
        return _name;
      };
    public:
      ITestExecutor(std::string name){ this->_name = fixName(name); };
      virtual bool execute(){ return false; };
    };    

    template<typename TTest>
    class UnitTest{
      template<typename T, typename U>
      class TargetTestExecutor: public ITestExecutor{
      private:
        testGenerator<T> _scenario;
        TEST_OP _op;
        targetTestEvaluator<T, U> _evaluator;
        U _target;
      public:
        TargetTestExecutor(std::string name, testGenerator<T> scenario, TEST_OP op, targetTestEvaluator<T, U> evaluator, U target)
        : ITestExecutor(name){
          this->_scenario = scenario;
          this->_op = op;
          this->_evaluator = evaluator;
          this->_target = target;
        };
        bool execute(){
          LOG("RUN  [" + this->_name + "]");
          if (this->_scenario == NULL){
            LOG("TEST [" + this->_name + "]", 1, "NO_SCENARIO");
            return false;
          };
          LOG("exec scenario", 2);
          T _eval = this->_scenario();
          LOG("scenario executed", 2);
          LOG("exec evaluator", 2);
          bool _test = this->_evaluator(this->_op, _eval, this->_target);
          return LOG<bool>("TEST [" + this->_name + "]", _test, 1, _test ? "GOOD" : "BAD");
        };
      };
      template<typename T>
      class CustomTestExecutor: public ITestExecutor{
      private:
        testGenerator<T> _scenario;
        TEST_OP _op;
        testEvaluator<T> _evaluator;
      public:
        CustomTestExecutor(std::string name, testGenerator<T> scenario, TEST_OP op, testEvaluator<T> evaluator)
        : ITestExecutor(name){
          this->_scenario = scenario;
          this->_op = op;
          this->_evaluator = evaluator;
        };
        bool execute(){
          LOG("RUN  [" + this->_name + "]");
          if (this->_scenario == NULL){
            LOG("TEST [" + this->_name + "]", 1, "NO_SCENARIO");
            return false;
          };
          LOG("exec scenario", 2);
          T _eval = this->_scenario();
          LOG("scenario executed", 2);
          LOG("exec evaluator", 2);
          bool _test = this->_evaluator(this->_op, _eval);
          return LOG<bool>("TEST [" + this->_name + "]", _test, 1, _test ? "GOOD" : "BAD");
        };
      };
    private:
      std::string _name;
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
      UnitTest(std::string name, typename enableIf<testCompatible<TTest, TTest>::value, TTest>::type* = 0){
        this->_name = name;
        this->_scenario = NULL;
      };
      UnitTest<TTest>* scenario(testGenerator<TTest> func){
        this->_scenario = func;
        return this;
      };
      ITestExecutor* evalWith(testEvaluator<TTest> func, TEST_OP op = TEST_EQUAL){
        return new CustomTestExecutor<TTest>(this->_name, this->_scenario, op, func);
      };
      template<typename TTarget>
      typename enableIf<testCompatible<TTest, TTarget>::value && 
      (isComparable<TTarget>::value || isNative<TTarget>::value), ITestExecutor>::type*
      evalWith(TEST_OP op, TTarget value){
        return new TargetTestExecutor<TTest, TTarget>(this->_name, this->_scenario, op, &UnitTest::comparableEvaluator, value);
      };
      template<typename TTarget>
      typename enableIf<testCompatible<TTest, TTarget>::value && (!isComparable<TTarget>::value && !isNative<TTarget>::value), ITestExecutor>::type*
      evalWith(TEST_OP op, TTarget value){
        return new TargetTestExecutor<TTest, TTarget>(this->_name, this->_scenario, op, &UnitTest::objectEvaluator, value);
      };
    };
  };
};