#pragma once

#include <string>
#include "../node-Base.hpp"
#include "../Logger/node-Logger.hpp"

namespace dNode{
  namespace Module{
    class ObjectInfo: public dNode::Dictionary<std::string, dNode::Variant*>{
    public:
      ObjectInfo(): Dictionary<std::string, dNode::Variant*>(){ };
      template<typename T>
      typename enableIf<isBaseOf<ObjectInfo, T>::value, bool>::type equal(T* obj){
        LOG("execute ObjectInfor equality", 2);
        if (obj == NULL) return false;
        return dNode::Dictionary<std::string, dNode::Variant*>::equal(static_cast<Dictionary<std::string, dNode::Variant*>*>(obj));
      };
    };
    class ArrayInfo: public dNode::List<dNode::Variant*>{
    public:
      ArrayInfo(): List<dNode::Variant*>(){ };
    };
    
    class ExecuteInfo : public Object{
    private:
      std::string _module;
      std::string _method;
      dNode::Variant* _params;
    public:
      ExecuteInfo(std::string module, std::string method, dNode::Variant* params = NULL)
      : Object(){
        this->_module = module;
        this->_method = method;
        this->_params = NULL;
        if (params != NULL) this->_params = params;
      };
      std::string getModule(){ return this->_module; };
      std::string getMethod(){ return this->_method; };
      dNode::Variant* getParams(){ return this->_params; };

      template<typename T>
      typename enableIf<isBaseOf<ExecuteInfo, T>::value, bool>::type equal(T* obj){
        if (obj == NULL) return false;
        ExecuteInfo* _cmp = static_cast<ExecuteInfo*>(obj);
        if (this->_module != _cmp->getModule()) return false;
        if (this->_method != _cmp->getMethod()) return false;
        if ((this->_params == NULL && _cmp->getParams() != NULL) || (this->_params != NULL && _cmp->getParams() == NULL)) return false;
        if (this->_params != NULL) return val(this->_params) == val(_cmp->getParams());
        else return true;
      };
    };

    class ResultData; // Forward
    class ResultInfo: public Object{
    private:
      ExecuteInfo* _execInfo;
      ResultData* _data;
    public:
      ResultInfo(ExecuteInfo* execInfo, ResultData* data): Object(){
        this->_execInfo = execInfo;
        this->_data = data;
      };
    };

    class ResultData: public ObjectInfo{
    private:
      std::string _type;
    public:
      ResultData(std::string type): ObjectInfo(){ this->_type = type; };
    };
    class ExceptionData: public ResultData{
    public:
      ExceptionData(std::string type, std::string message, std::string details = "")
      : ResultData(type){
        this->add("message", var(message));
        this->add("details", var(details));
        this->lock();
      };
    };
    ExceptionData* nullReferenceException(std::string field, std::string message = "", std::string details = ""){
      if (message.empty() || message == "") message = "NULL value access";
      ExceptionData* _data = new ExceptionData("NullReferenceException", message, details);
      _data->add("field", var(field));
      return _data;
    };
    ExceptionData* methodNotFoundException(std::string module, std::string method, std::string message = "", std::string details = ""){
      if (message.empty() || message == "") message = "Method not found in member";
      ExceptionData* _data = new ExceptionData("MethodNotFoundException", message, details);
      _data->add("module", var(module));
      _data->add("method", var(method));
      return _data;
    };
    ExceptionData* invalidExecutionException(std::string forModule, std::string inModule, std::string message = "", std::string details = ""){
      if (message.empty() || message == "") message = "Invalid execution in module";
      ExceptionData* _data = new ExceptionData(message, details);
      _data->add("for", var(forModule));
      _data->add("in", var(inModule));
      return _data;
    };

    class ModuleBase: public Object{
    private:
      std::string _moduleName;
      std::string _moduleVersion;
      ResultData* noHandler(ExecuteInfo* executeInfo){ return methodNotFoundException(executeInfo->getModule(), executeInfo->getMethod()); };
      ResultData* moduleInfoHandler(ExecuteInfo* executeInfo = 0){
        ResultData* _data = new ResultData("ModuleInfo");
        _data->add("name", var(this->getModuleName()));
        _data->add("version", var(this->getModuleVersion()));
        return _data;
      };
    protected:
      typedef ResultData*(ModuleBase::*execHandler)(ExecuteInfo*);
      virtual execHandler getExecHandler(std::string method){
        if (method == "moduleInfo") return &ModuleBase::moduleInfoHandler;
        else return &ModuleBase::noHandler;
      };
    public:
      ResultInfo* execute(ExecuteInfo* executeInfo){
        ResultData* _data = NULL;
        if (executeInfo != NULL && executeInfo->getModule() == this->getModuleName())
          _data = (this->*getExecHandler(executeInfo->getMethod()))(executeInfo);
        else if (executeInfo == NULL) _data = nullReferenceException("executeInfo", "", "error on: ModuleBase::execute(ExecuteInfo*)");
        else if (executeInfo->getModule() != this->getModuleName())
          _data = invalidExecutionException(executeInfo->getModule(), this->getModuleName(), "", "error on: ModuleBase::execute(ExecuteInfo*)");
        return new ResultInfo(executeInfo, _data);
      };
      std::string getModuleName(){ return this->_moduleName; };
      std::string getModuleVersion(){ return this->_moduleVersion; };
    };

    // class JSONArgument: public Variant
    // , public JSONObject{
    // public:
    //   JSONArgument(JsonVariant json)
    //   : Variant(){
    //     if (!this->fromJSON(json)) this->_type = TYPE_INVALID;
    //   };
    //   bool fromJSON(JsonVariant json){
    //     if (json.success()){
    //       if (json.is<JsonArray>()){
    //         dNode::List<JSONArgument*>* _arr = new dNode::List<JSONArgument*>();
    //         for (auto _el: json.as<JsonArray>()){
    //           JSONArgument* _arg = new JSONArgument(_el);
    //           if (_arg->valid()) _arr->add(_arg);
    //           else return false;
    //         }
    //         this->set(_arr);
    //       }
    //       else if (json.is<JsonObject>()){
    //       }
    //       else if (json.is<bool>()) this->set(json.as<bool>());
    //       else if (json.is<unsigned long>()) this->set(json.as<unsigned long>());
    //       else if (json.is<float>()) this->set(json.as<float>());
    //       else if (json.is<const char*>()) this->set(json.as<const char*>());
    //       else return false;
    //     }
    //     else return false;
    //   };
    //   void fillJSON(JsonVariant json){};
    // };
    // class JSONParam: public JSONObject{};
    // class JSONInvoker: public Dictionary<std::string, JSONParam>
    // , public JSONObject{

    // };
  };
};