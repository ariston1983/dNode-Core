#pragma once

#include <string>
#include "../dNode/node-Base.hpp"
#include "../dNode/Logger/node-Logger.hpp"
#include "../dNode/Test/node-Tester.hpp"
#include "../dNode/Module/node-Module.hpp"

using namespace dNode;
using namespace dNode::UnitTest;
using namespace dNode::Module;

namespace Test{
  namespace Module{
    namespace Base{
      ObjectInfo* createObjectInfo(){
        ObjectInfo* _obj = new ObjectInfo();
        _obj->add("name", dNode::var("Mike"));
        _obj->add("age", dNode::var(30));
        return _obj;
      };
      dNode::Variant* createParams(){ return dNode::var(createObjectInfo()); };
      class Request: public ExecuteInfo{
      public:
        Request(std::string module, std::string method, dNode::Variant* params = NULL)
        : ExecuteInfo(module, method, params){ };
      };

      void EXECUTE(){
        RUN_TEST<ObjectInfo*>("TEST_MODULE_OBJECTINFO")
        ->scenario([](){ return createObjectInfo(); })
        ->evalWith([](TEST_OP op, ObjectInfo* value){
          if (value == NULL){ LOG("missing value", 2); return false; };
          if (val(value) != val(createObjectInfo())){ LOG("invalid value", 2); return false; };
          return true;
        })
        ->execute();

        RUN_TEST<Request*>("TEST_MODULE_EXECUTEINFO")
        ->scenario([](){ return new Request("Test_Module", "Test_Method", createParams()); })
        ->evalWith([](TEST_OP op, Request* value){
          if (value == NULL){ LOG("missing value", 2); return false; }
          if (value->getModule() != "Test_Module"){ LOG("invalid module", 2); return false; }
          if (value->getMethod() != "Test_Method"){ LOG("invalid method", 2); return false; }
          if (value->getParams() == NULL){ LOG("missing params", 2); return false; }
          if (val(value->getParams()) != val(createParams())){ LOG("invalid params", 2); return false; }
          return true;
        })
        ->execute();
      };
    };
  };
};