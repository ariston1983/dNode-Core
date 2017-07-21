#pragma once

#include <string>
#include <map>
#include "node-Helper.hpp"

namespace dNode{
  class Object;
  class Variant;

  namespace Reflection{
    template<class TClass>
    typename enableIf<isBaseOf<Object, typename TClass>::value, typename TClass>::type* create(Variant* param = NULL){
      if (param == NULL) return new TClass();
      else return new TClass(param);
    };

    class Reflection{
      typedef std::map<std::string, Object*(*)(Variant*)> map_Type;
    public:
      template<typename TClass>
      static bool reg(const std::string& fullName){ return false; };
    };
  };
};