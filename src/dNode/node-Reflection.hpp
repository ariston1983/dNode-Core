#pragma once

#include <string>
#include <map>
#include "node-Helper.hpp"

namespace dNode{
  class Object;
  class Variant;

  namespace Reflection{
    #define OBJECT_TYPEINFO(ClassId) \
    public: \
      static std::string& getTypeId(){ \
        static std::string _typeId(ClassId); \
        return _typeId; \
      };
    #define REFLECTION_TYPEINFO(ThisClass, BaseClass) \
    public: \
      virtual std::string& getType(){ return ThisClass::getTypeId(); }; \
      virtual bool assignableFrom(Object& obj){ return obj.isSubclassOf(this->getType()); }; \
      virtual bool isSubclassOf(const std::string& typeId){ return typeId == ThisClass::getTypeId() || BaseClass::isSubclassOf(typeId); };

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