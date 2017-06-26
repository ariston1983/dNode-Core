#pragma once

#include <string>

class nodeLogger{
  typedef void (*logPrinter)(std::string, int, std::string);
private:
  static logPrinter _printer;
  static logPrinter getPrinter(){
    if (_printer == NULL) _printer = &nodeLogger::emptyPrinter;
    return _printer;
  };
public:
  static void emptyPrinter(std::string message, int code, std::string data){ };
  static void init(logPrinter printer){ _printer = printer; };
  static void log(std::string message, int code = 0, std::string data = ""){
    getPrinter()(message, code, data);
  };
  template<typename TResult>
  static TResult log(std::string message, TResult pass, int code = 0, std::string data = ""){
    getPrinter()(message, code, data);
    return pass;
  };
};
void LOG(std::string message, int code = 0, std::string data = ""){
  nodeLogger::log(message, code, data);
};
template<typename TResult>
TResult LOG(std::string message, TResult pass, int code = 0, std::string data = ""){
  nodeLogger::log(message, code, data);
  return pass;
};

#ifndef __NODE_DEFAULT_LOGGER__
#define __NODE_DEFAULT_LOGGER__
nodeLogger::logPrinter nodeLogger::_printer = &nodeLogger::emptyPrinter;
#endif