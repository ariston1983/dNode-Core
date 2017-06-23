#pragma once

#include "Arduino.h"
#include <string>
#include "node-Logger.hpp"

class nodeTester{
  template<typename TTestResult>
  class testUnit{
    typedef TTestResult(*testFunc)();
  private:
    std::string _name;
    testFunc _function;
    TTestResult _target;
  public:
    static testUnit<TTestResult>* create(std::string name, testFunc function = NULL){ return new testUnit<TTestResult>(name, function); };
    testUnit(std::string name, testFunc function = NULL){
      this->_name = name;
      this->_function = function;
    };
    testUnit<TTestResult>* exec(testFunc function){
      this->_function = function;
      return this;
    };
    testUnit<TTestResult>* target(TTestResult value){
      this->_target = value;
      return this;
    };
    bool run(){
      if (this->_function == NULL){
        return false;
      }
      TTestResult _eval = this->_function();
      bool _res = (_eval == this->_target);
      nodeLogger::log("Test ["+this->_name+"]", 0, _res ? "Match" : "Unmatch");
      return _res;
    };
  };
public:
  static void testLogger(std::string message, int code, std::string data){
    std::string _print = "LOG: " + message;
    if (code != 0)
      _print += " [Code: ";
    Serial.print(_print.c_str());
    if (code != 0){
      Serial.print(code);
      Serial.println("]");
    }
    if (!data.empty() && data != ""){
      _print = "\tDATA: " + data;
      Serial.println(_print.c_str());
    }
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
