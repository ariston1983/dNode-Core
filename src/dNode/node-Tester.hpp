#pragma once

#include "Arduino.h"
#include <string>
#include "node-Logger.hpp"

class nodeTester{
  template<typename TUnitType>
  class testUnit{
    typedef TUnitType(*testFunc)();
  private:
    std::string _name;
    testFunc _function;
    testFunc _comparer;
    TUnitType _target;
  public:
    static testUnit<TUnitType>* create(std::string name, testFunc function = NULL){ return new testUnit<TUnitType>(name, function); };
    testUnit(std::string name, testFunc function = NULL){
      this->_name = name;
      this->_function = function;
      this->_comparer = NULL;
    };
    testUnit<TUnitType>* eval(testFunc function){
      this->_function = function;
      return this;
    };
    testUnit<TUnitType>* equal(testFunc function){
      this->_comparer = function;
      return this;
    };
    testUnit<TUnitType>* equal(TUnitType value){
      this->_target = value;
      return this;
    };
    bool run(){
      nodeLogger::log("TEST [" + this->_name + "]");
      if (this->_function == NULL){
        nodeLogger::log("RESULT: no eval function");
        return false;
      }
      nodeLogger::log("...exec eval function");
      TUnitType _eval = this->_function();
      nodeLogger::log("...get target value");
      TUnitType _target = this->_target;
      if (this->_comparer != NULL){
        nodeLogger::log("...exec compare function");
        _target = this->_comparer();
      }
      nodeLogger::log("...compare value");
      bool _res = (_eval == _target);
      nodeLogger::log(_res ? "RESULT: match" : "RESULT: unmatch");
      return _res;
    };
  };
public:
  static void testLogger(std::string message, int code, std::string data){
    Serial.println();
    Serial.print(message.c_str());
    Serial.print(" [HEAP: ");
    Serial.print(ESP.getFreeHeap());
    Serial.print("]");
  };
  static void init(){
    nodeLogger::init(&nodeTester::testLogger);
    nodeLogger::log("Tester initialized");
  };
  template<typename TBeginTarget>
  static testUnit<TBeginTarget>* begin(std::string name){
    return testUnit<TBeginTarget>::create(name);
  };
  template<typename TRunTarget>
  static bool run(std::string name, TRunTarget(*function)(), TRunTarget target){
    return testUnit<TRunTarget>::crate(name, function)->target(target)->run();
  };
};
template<typename T>
nodeTester::testUnit<T>* RUN_TEST(std::string name = "noname"){
  return nodeTester::begin<T>(name);
};