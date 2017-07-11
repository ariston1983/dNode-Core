#pragma once

#include "Arduino.h"
#include <string>
#include <vector>
#include <map>
#include "node-Helper.hpp"

/****************************************************************************
 * Base interfaces
 ****************************************************************************/
namespace dNode{
  class Object{
  private:
    bool _empty;
  public:
    Object(){ this->_empty = true; };
    static Object* empty(){ return new Object(); };
    template<typename T>
    friend typename enableIf<isBaseOf<dNode::Object, T>::value, bool>::type operator==(T& lhs, T& rhs){ return lhs.equal(ptr(rhs)); };
    template<typename T>
    friend typename enableIf<isBaseOf<dNode::Object, T>::value, bool>::type operator!=(T& lhs, T& rhs){ return !lhs.equal(ptr(rhs)); };
    virtual bool equal(Object* obj){ return false; };
    virtual std::string toString(){ return ""; };
  };

  class JSONObject: public Object{
  public:
    static std::string stringify(JsonVariant json){
      int _len = json.measureLength()+1;
      char _store[_len];
      json.printTo(_store, _len);
      return std::string(_store);
    };
    virtual bool fromJSON(JsonVariant json){ return false; };
    virtual bool fromJSON(std::string str){
      DynamicJsonBuffer _buffer(str.length());
      JsonVariant _obj = _buffer.parseObject(str.c_str());
      if (_obj.success()) return this->fromJSON(_obj);
      else return false;
    };
    virtual void fillJSON(JsonVariant json){ };
    virtual std::string toJSON(){
      DynamicJsonBuffer _buffer(512);
      JsonObject& _obj = _buffer.createObject();
      this->fillJSON(_obj);
      return stringify(_obj);
    };
  };

  template<typename TItem, typename TReturn>
  using foreach_List = void(*)(Object*, TItem, bool&, TReturn&);
  template<typename TKey, typename TValue, typename TReturn>
  using foreach_Dictionary = void(*)(Object*, TKey, TValue, bool&, TReturn&);

  template<typename T>
  class List: public dNode::Object{
    typedef std::vector<T> list_Type;
  private:
    bool _locked;
    list_Type* _list;
    list_Type* getList(){
      if (this->_list == NULL) this->_list = new list_Type();
      return this->_list;
    };
  protected:
    void lock(){ this->_locked = true; };
  public:
    List(): Object(){
      this->_list = NULL;
      this->_locked = false;
    };
    ~List(){
      this->_locked = false;
      this->clear();
    };
    int count(){ return this->getList()->size(); };
    bool add(T value){
      if (this->_locked) return false;
      this->getList()->push_back(value);
      return true;
    };
    bool insert(int index, T value){
      if (this->_locked) return false;
      if (index == 0){
        if (this->count() > 0) this->getList()->push_front(value);
        else this->getList()->push_back(value);
        return true;
      }
      else if (index != 0 && index >= this->count()){
        this->getList()->push_back(value);
        return true;
      }
      else{
        int _curr = 0;
        for (typename list_Type::iterator _it = this->getList()->begin(); _it != this->getList()->end(); ++_it){
          if (_curr == index){
            this->getList()->insert(_it, value);
            return true;
          }
          _curr++;
        }
      }
    };
    bool remove(T value){
      if (this->_locked) return false;
      this->getList()->remove(value);
      return true;
    };
    bool removeAt(int index){
      if (this->_locked) return false;
      if (index >= 0 && index < this->count()){
        int _curr = 0;
        for (typename list_Type::iterator _it = this->getList()->begin(); _it != this->getList()->end(); ++_it){
          if (_curr == index){
            if (isPointer<T>::value) delete_if_pointer(*_it);
            this->getList()->erase(_it);
            return true;
          }
          _curr++;
        }
      }
      return false;
    };
    bool clear(){
      if (this->_locked) return false;
      if (isPointer<T>::value)
        for (typename list_Type::iterator _it = this->getList()->begin(); _it != this->getList()->end(); ++_it)
          delete_if_pointer(*_it);
      this->getList()->clear();
    };
    template<typename TReturn>
    void forEach(foreach_List<T, TReturn> action, TReturn& ret){
      bool _abort = false;
      for (typename list_Type::iterator _it = this->getList()->begin(); _it != this->getList()->end(); ++_it){
        action(this, *_it, _abort, ret);
        if (_abort) break;
      }
    };
    T operator[](int index){
      if (index >= 0 && index < this->count()){
        int _curr = 0;
        for (typename list_Type::iterator _it = this->getList()->begin(); _it != this->getList()->end(); ++_it){
          if (_curr == index) return *_it;
          _curr++;
        }
      }
      return T();
    };

    // virtual bool equal(const dNode::Object& obj){
    //   if (obj == NULL) return false;
    //   dNode::List<T> _cmp = static_cast<dNode::List<T>&>(obj);
    //   if (_cmp){
    //     if (_cmp->count() != this->count()) return false;
    //     for (int _i = 0; _i < this->count(); _i++){
    //       T _v1 = static_cast<T>((*this)[_i]);
    //       T _v2 = static_cast<T>((*_cmp)[_i]);
    //       if (cast_value(_v1) != cast_value(_v2)) return false;
    //     }
    //     return true;
    //   }
    //   else return false;
    // };
  };
  
  template<typename TKey, typename TValue>
  class Dictionary: public dNode::Object{
    typedef std::map<TKey, TValue> dict_Type;
  private:
    bool _locked;
    dict_Type* _map;
    dict_Type* getMap(){
      if (this->_map == NULL) this->_map = new dict_Type();
      return this->_map;
    };
  protected:
    void lock(){ this->_locked = true; };
  public:
    Dictionary(): Object(){
      this->_map = NULL;
      this->_locked = false;
    };
    ~Dictionary(){
      this->_locked = false;
      this->clear();
    };
    int count(){ return this->getMap()->count(); };
    std::vector<TKey>* getKeys(){
      std::vector<TKey>* _keys = new std::vector<TKey>();
      for (typename dict_Type::iterator _it = this->getMap()->begin(); _it != this->getMap()->end(); ++_it)
        _keys->push_back(_it->first);
      return _keys;
    };
    bool has(TKey key){
      typename dict_Type::iterator _it = this->getMap()->find(key);
      return (_it != this->getMap()->end());
    };
    bool add(TKey key, TValue value){
      if (this->_locked) return false;
      typename dict_Type::iterator _it = this->getMap()->find(key);
      if (_it == this->getMap()->end()) this->getMap()->insert(std::make_pair(key, value));
      else _it->second = value;
      return true;
    };
    bool remove(TKey key){
      if (this->_locked) return false;
      typename dict_Type::iterator _it = this->getMap()->find(key);
      if (_it != this->getMap()->end()){
        delete_if_pointer(_it->first);
        delete_if_pointer(_it->second);
        this->getMap()->erase(_it);
        return true;
      }
      else return false;
    };
    void clear(){
      if (this->_locked) return;
      for (typename dict_Type::iterator _it = this->getMap()->begin(); _it != this->getMap()->end(); ++_it){
        delete_if_pointer(_it->first);
        delete_if_pointer(_it->second);
        this->getMap()->erase(_it);
      }
    };
    template<typename TReturn>
    void forEach(foreach_Dictionary<TKey, TValue, TReturn> action, TReturn& ret){
      bool _abort = false;
      for (typename dict_Type::iterator _it = this->getMap()->begin(); _it != this->getMap()->end(); ++_it){
        action(this, _it->first, _it->second, _abort, ret);
        if (_abort) break;
      }
    };
    TValue operator[](TKey key){
      typename dict_Type::iterator _it = this->getMap()->find(key);
      return (_it != this->getMap()->end()) ? _it->second : TValue();
    };
  };
  
  class Variant : public Object{
    union Value{
      unsigned long asInteger;
      float asFloat;
      const char* asChars;
      Object* asObject;
    };
  protected:
    enum Type{
      TYPE_INVALID,
      TYPE_BOOL,
      TYPE_INTEGER,
      TYPE_FLOAT,
      TYPE_CHARS,
      TYPE_OBJECT
    };
    Type _type;
    Value _value;
  public:
    Variant(){ this->_type == TYPE_INVALID; };
    template<typename T>
    Variant(T value, typename enableIf<isNative<T>::value>::type* = 0){
      this->_type = TYPE_INVALID;
      this->set<T>(value);
    };
    Variant(dNode::Object* value){
      this->_type = TYPE_INVALID;
      if (value != NULL)
        this->set(value);
    };
    ~Variant(){
      if (this->_type == TYPE_OBJECT) delete_if_pointer(this->_value.asObject);
    };

    bool valid(){ return this->_type != TYPE_INVALID; };

    template<typename T>
    bool set(T value, typename enableIf<isInteger<T>::value>::type* = 0){
      if (this->_type == TYPE_INVALID){
        this->_value.asInteger = static_cast<unsigned long>(value);
        if (isBool<T>::value) this->_type = TYPE_BOOL;
        else this->_type = TYPE_INTEGER;
        return true;
      }
      else if ((this->_type == TYPE_BOOL && isBool<T>::value) || this->_type == TYPE_INTEGER)
        this->_value.asInteger = static_cast<unsigned long>(value);
      else return false;
    };
    template<typename T>
    bool set(T value, typename enableIf<isFloat<T>::value>::type* = 0){
      if (this->_type == TYPE_INVALID || this->_type == TYPE_FLOAT){
        this->_value.asFloat = static_cast<float>(value);
        this->_type = TYPE_FLOAT;
        return true;
      }
      else return false;
    };
    template<typename T>
    bool set(T value, typename enableIf<isChars<T>::value>::type* = 0){
      if (this->_type == TYPE_INVALID || this->_type == TYPE_CHARS){
        this->_value.asChars = static_cast<const char*>(value);
        this->_type = TYPE_CHARS;
        return true;
      }
      else return false;
    };
    template<typename T>
    bool set(T value, typename enableIf<isString<T>::value>::type* = 0){
      if (this->_type == TYPE_INVALID || this->_type == TYPE_CHARS){
        this->_value.asChars = static_cast<std::string>(value).c_str();
        this->_type = TYPE_CHARS;
        return true;
      }
      else return false;
    };
    bool set(dNode::Object* value){
      if (this->_type == TYPE_INVALID || this->_type == TYPE_OBJECT){
        this->_value.asObject = value;
        this->_type = TYPE_OBJECT;
        return true;
      }
      else return false;
    };

    template<typename T>
    typename enableIf<isBool<T>::value, bool>::type is(){ return this->valid() && this->_type == TYPE_BOOL; };
    template<typename T>
    typename enableIf<isInteger<T>::value, bool>::type is(){ return this->valid() && this->_type == TYPE_INTEGER; };
    template<typename T>
    typename enableIf<isFloat<T>::value, bool>::type is(){ return this->valid() && this->_type == TYPE_FLOAT; };
    template<typename T>
    typename enableIf<isChars<T>::value, bool>::type is(){ return this->valid() && this->_type == TYPE_CHARS; };
    template<typename T>
    typename enableIf<isString<T>::value, bool>::type is(){ return this->valid() && this->_type == TYPE_CHARS; };
    template<typename T>
    typename enableIf<isBaseOf<dNode::Object, T>::value && isPointer<T>::value, bool>::type is(){
      return this->valid() && this->_type == TYPE_OBJECT && dynamic_cast<typename clearClass<T>::type*>(this->_value.asObject);
    };
    
    template<typename T>
    typename enableIf<isBool<T>::value, bool>::type as() const{
      if (this->_type == TYPE_BOOL) return static_cast<bool>(this->_value.asInteger);
      else return T();
    };
    template<typename T>
    typename enableIf<isInteger<T>::value, T>::type as() const{
      if (this->_type == TYPE_INTEGER) return static_cast<T>(this->_value.asInteger);
      else return T();
    };
    template<typename T>
    typename enableIf<isChars<T>::value && !isString<T>::value, const char*>::type as() const{
      if (this->_type == TYPE_CHARS) return this->_value.asChars;
      else return "";
    };
    template<typename T>
    typename enableIf<!isChars<T>::value && isString<T>::value, std::string>::type as() const{
      if (this->_type == TYPE_CHARS) return std::string(this->_value.asChars);
      else return "";
    };
    template<typename T>
    typename enableIf<isBaseOf<dNode::Object, T>::value, T*>::type as(){
      if (this->_type == TYPE_OBJECT) return static_cast<typename clearPointer<T>::type*>(this->_value.asObject);
      else return NULL;
    };

    template<typename T>
    operator T() const{ return this->as<T>(); };
    template<typename T>
    typename enableIf<isNative<T>::value, T>::type operator=(T value){
      this->set(value);
      return this->as<T>();
    };
    template<typename T>
    typename enableIf<isBaseOf<dNode::Object, T>::value && isPointer<T>::value,
      typename clearPointer<T>::type*>::type operator=(dNode::Object* value){
      this->set(value);
      return this->as<T>();
    };
    
    template<typename T>
    bool equal(T obj, typename enableIf<isNative<T>::value>::type* = 0){
      return this->as<T>() == obj;
    };
    template<typename T>
    typename enableIf<isVariant<T>::value, bool>::type equal(T* obj){
      if (obj == NULL || this->_type != TYPE_OBJECT) return false;
      else{
        Variant* _var = static_cast<Variant*>(obj);
        return this->equal(_var->as<dNode::Object>());
      };
    };
    template<typename T>
    typename enableIf<isBaseOf<dNode::Object, T>::value && !isVariant<T>::value, bool>::type equal(T* obj){
      if (obj == NULL || this->_type != TYPE_OBJECT) return false;
      else return this->_value.asObject->equal(obj);
    };
  };
  template<typename T>
  Variant* var(T value, typename enableIf<isNative<T>::value>::type* = 0){ return new Variant(value); };
  Variant* var(dNode::Object* value){ return new Variant(value); };
};
/****************************************************************************
 * Invoker interfaces
 ****************************************************************************/
namespace dNode{
  namespace Module{
    class ObjectInfo: public dNode::Dictionary<std::string, dNode::Variant*>{};
    class ArrayInfo: public dNode::List<dNode::Variant*>{};
    
    class ExecuteInfo : public ObjectInfo{
    public:
      ExecuteInfo(std::string module, std::string method, Variant* params)
      : ObjectInfo(){
        this->add("module", var(module));
        this->add("method", var(method));
        if (params != NULL) this->add("params", params);
        this->lock();
      };
      std::string getModule(){ return (*this)["module"]->as<std::string>(); };
      std::string getMethod(){ return (*this)["module"]->as<std::string>(); };
      Variant* getParams(){ return this->has("params") ? (*this)["params"] : NULL; };
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
// class InvokerArgument{
//   enum ArgumentType{
//     TYPE_INVALID,
//     TYPE_BOOL,
//     TYPE_INTEGER,
//     TYPE_FLOATING,
//     TYPE_CHARS
//   };
//   union ArgumentValue{
//     bool asBool;
//     unsigned long asInteger;
//     float asFloat;
//     const char* asChars;
//   };
// private:
//   std::string _moduleName;
//   ArgumentType _type;
//   ArgumentValue _value;
// public:
//   InvokerArgument(){
//     this->_moduleName = "";
//     this->_type = TYPE_INVALID;
//   };
//   InvokerArgument(std::string name, JsonVariant value){
//     this->_moduleName = name;
//     this->_type = TYPE_INVALID;
//     if (isJSONBool(value)) this->set<bool>(value.as<bool>());
//     else if (isJSONInteger(value)) this->set<unsigned int>(value.as<unsigned int>());
//     else if (isJSONFloat(value)) this->set<float>(value.as<float>());
//     else if (isJSONChars(value)) this->set<const char*>(value.as<const char*>());
//   };
//   template<typename T>
//   InvokerArgument(std::string name, T value, typename enableIf<isBool<T>::value ||
//     isInteger<T>::value ||
//     isFloat<T>::value ||
//     isChars<T>::value ||
//     isString<T>::value>::type* = 0){
//     this->_moduleName = name;
//     this->_type = TYPE_INVALID;
//     this->set<T>(value);
//   };
//   static InvokerArgument& invalid(){
//     static InvokerArgument instance;
//     return instance;
//   };
//   std::string getName(){ return this->_moduleName; };
//   bool valid(){ return !this->_moduleName.empty() && this->_moduleName != "" && this->_type != TYPE_INVALID; };
//   template<typename T>
//   T as(typename enableIf<isBool<T>::value>::type* = 0){
//     if (this->_type == TYPE_BOOL) return T(this->_value.asBool);
//     else return 0;
//   };
//   template<typename T>
//   T as(typename enableIf<isInteger<T>::value>::type* = 0){
//     if (this->_type == TYPE_INTEGER) return T(this->_value.asInteger);
//     else return 0;
//   };
//   template<typename T>
//   T as(typename enableIf<isFloat<T>::value>::type* = 0){
//     if (this->_type == TYPE_FLOATING) return T(this->_value.asFloat);
//     else return 0;
//   };
//   template<typename T>
//   T as(typename enableIf<isChars<T>::value>::type* = 0){
//     if (this->_type == TYPE_CHARS) return T(this->_value.asChars);
//     else return "";
//   };
//   template<typename T>
//   T as(typename enableIf<isString<T>::value>::type* = 0){
//     if (this->_type == TYPE_CHARS) return std::string(this->_value.asChars);
//     else return "";
//   };
//   template<typename T>
//   bool set(T value, typename enableIf<isBool<T>::value>::type* = 0){
//     if (this->_type == TYPE_INVALID || this->_type == TYPE_BOOL){
//       this->_type = TYPE_BOOL;
//       this->_value.asBool = static_cast<bool>(value);
//       return true;
//     }
//     else return false;
//   };
//   template<typename T>
//   bool set(T value, typename enableIf<isInteger<T>::value>::type* = 0){
//     if (this->_type == TYPE_INVALID || this->_type == TYPE_INTEGER){
//       this->_type = TYPE_INTEGER;
//       this->_value.asInteger = static_cast<unsigned int>(value);
//       return true;
//     }
//     else return false;
//   };
//   template<typename T>
//   bool set(T value, typename enableIf<isFloat<T>::value>::type* = 0){
//     if (this->_type == TYPE_INVALID || this->_type == TYPE_FLOATING){
//       this->_type = TYPE_FLOATING;
//       this->_value.asFloat = static_cast<float>(value);
//       return true;
//     }
//     else return false;
//   };
//   template<typename T>
//   bool set(T value, typename enableIf<isChars<T>::value>::type* = 0){
//     if (this->_type == TYPE_INVALID || this->_type == TYPE_CHARS){
//       this->_type = TYPE_CHARS;
//       this->_value.asChars = static_cast<const char*>(value);
//       return true;
//     }
//     else return false;
//   };
//   template<typename T>
//   bool set(T value, typename enableIf<isString<T>::value>::type* = 0){
//     if (this->_type == TYPE_INVALID || this->_type == TYPE_CHARS){
//       this->_type = TYPE_CHARS;
//       this->_value.asChars = static_cast<std::string>(value).c_str();
//       return true;
//     }
//     else return false;
//   };
//   template<typename T>
//   typename enableIf<isBool<T>::value ||
//     isInteger<T>::value ||
//     isFloat<T>::value ||
//     isString<T>::value, InvokerArgument&>::type operator=(const T& value){
//     this->set<T>(value);
//     return *this;
//   };
//   template<typename T>
//   typename enableIf<isChars<T>::value, InvokerArgument&>::type operator=(const T* value){
//     this->set<T>(value);
//     return *this;
//   };
//   template<typename T>
//   operator T(){
//     return this->as<T>();
//   };
// };
// class Invoker{
//   typedef std::map<std::string, InvokerArgument*> args_Type;
// private:
//   std::string _module;
//   std::string _method;
//   args_Type* _args;
//   args_Type* getArgs(){
//     if (this->_args == NULL) this->_args = new args_Type();
//     return this->_args;
//   };
// public:
//   Invoker(std::string module, std::string method){
//     this->_module = module;
//     this->_method = method;
//     this->_args = NULL;
//   };
//   ~Invoker(){
//     for (auto _it = this->getArgs()->begin(); _it != this->getArgs()->end(); ++_it){
//       delete _it->second;
//       this->getArgs()->erase(_it);
//     }
//     delete this->_args;
//   };
//   static Invoker& invalid(){
//     static Invoker instance("", "");
//     return instance;
//   };
//   std::string getModule(){ return this->_module; };
//   std::string getMethod(){ return this->_method; };
//   std::vector<std::string>* getParamNames(){
//     std::vector<std::string>* _moduleNames = new std::vector<std::string>();
//     for (args_Type::iterator _it = this->getArgs()->begin(); _it != this->getArgs()->end(); ++_it)
//       _moduleNames->push_back(_it->first);
//     return _moduleNames;
//   };
//   template<typename T>
//   Invoker& add(std::string name, T value){
//     args_Type::iterator _it = this->getArgs()->find(name);
//     if (_it != this->getArgs()->end()){
//       if (_it->second->set<T>(value)) return *this;
//       else return Invoker::invalid();
//     }
//     this->getArgs()->insert(std::make_pair(name, new InvokerArgument(name, value)));
//     return *this;
//   };
//   int count(){ return this->getArgs()->size(); };
//   bool has(std::string name){ return this->getArgs()->find(name) != this->getArgs()->end(); };
//   InvokerArgument& get(std::string name){
//     args_Type::iterator _it = this->getArgs()->find(name);
//     if (_it != this->getArgs()->end()) return *_it->second;
//     else InvokerArgument::invalid();
//   };
//   InvokerArgument& operator[](std::string name){ return this->get(name); };
// };
// class IExecArgs: public IJSONSupport{
// public:
//   IExecArgs(){ };
//   virtual bool fromJSON(std::string json){ return true; };
//   virtual bool fromJSON(JsonVariant json){ return true; };
//   virtual void fillJSON(JsonVariant json){ };
//   template<typename T>
//   T* as(){ return static_cast<T*>(this); }
// };
// class ExecMeta: public IJSONSupport{
// private:
//   std::string _module;
//   std::string _method;
//   IExecArgs* _args;
// public:
//   ExecMeta(IExecArgs* args){
//     this->_args = NULL;
//     if (args != NULL) this->_args = args;
//   };
//   virtual bool fromJSON(std::string json){
//     DynamicJsonBuffer _buffer(json.length());
//     JsonVariant _var = _buffer.parseObject(json.c_str());
//     if (_var.success() && _var.is<JsonObject>()){
//       JsonObject& _obj = _var.as<JsonObject>();
//       if (!_obj.containsKey("module")) return false;
//       this->_module = (const char*)_obj["module"];
//       if (!_obj.containsKey("method")) return false;
//       this->_method = (const char*)_obj["method"];
//       if (_obj.containsKey("args") && _obj["args"].is<JsonObject>() && this->_args != NULL)
//         return this->_args->fromJSON(_obj["args"].as<JsonObject>());
//       else return true;
//     }
//     else return false;
//   };
//   virtual void fillJSON(JsonVariant json){
//     if (json.is<JsonArray>()) this->fillJSON(json.as<JsonArray>().createNestedObject());
//     else if (json.is<JsonObject>()){
//       JsonObject& _obj = json.as<JsonObject>();
//       _obj["module"] = strdup(this->_module.c_str());
//       _obj["method"] = strdup(this->_method.c_str());
//       JsonObject& _args = _obj.createNestedObject("args");
//       if (this->_args != NULL) this->_args->fillJSON(_args);
//     };
//   };
//   std::string getModule(){ return this->_module; };
//   std::string getMethod(){ return this->_method; };
//   IExecArgs* getArgs(){ return this->_args; };
//   template<typename T>
//   T* getArgsAs(){ return this->_args->as<T>(); };
// };
// template<typename TArgs>
// ExecMeta* execMeta(std::string json){
//   ExecMeta* _meta = new ExecMeta(new TArgs());
//   if (_meta->fromJSON(json)) return _meta;
//   else return NULL;
// };
// class IResultData: public IJSONSupport{
// private:
//   std::string _kind;
// protected:
//   IResultData(std::string kind){ this->_kind = kind; };
// public:
//   virtual void fillJSON(JsonVariant json){
//     if (json.is<JsonObject>()){
//       JsonObject& _obj = json.as<JsonObject>();
//       _obj["kind"] = this->_kind.c_str();
//     }
//   };
//   virtual std::string toJSON(){
//     DynamicJsonBuffer _buffer(512);
//     JsonObject& _obj = _buffer.createObject();
//     this->fillJSON(_obj);
//     return nodeJSON::stringify(_obj);
//   };
// };
// template<typename T>
// class NativeData: public IResultData{
// private:
//   std::string _type;
//   T _value;
// protected:
//   NativeData(std::string type, T value): IResultData("native"){
//     this->_type = type;
//     this->_value = value;
//   };
// public:
//   static NativeData<T>* create(std::string type, T value){
//     if (isNative<T>()){ return new NativeData(type, value); }
//     else return NULL;
//   };
//   virtual void fillJSON(JsonVariant json){
//     if (json.is<JsonObject>()){
//       IResultData::fillJSON(json);
//       JsonObject& _obj = json.as<JsonObject>();
//       _obj["type"] = this->_type.c_str();
//       _obj["value"] = this->_value;
//     }
//   };
// };
// template<>
// class NativeData<std::string>: public IResultData{
// private:
//   std::string _type;
//   std::string _value;
// protected:
//   NativeData(std::string type, std::string value): IResultData("native"){
//     this->_type = type;
//     this->_value = value;
//   };
// public:
//   static NativeData<std::string>* create(std::string type, std::string value){
//     return new NativeData<std::string>("string", value);
//   };
//   virtual void fillJSON(JsonVariant json){
//     if (json.is<JsonObject>()){
//       IResultData::fillJSON(json);
//       JsonObject& _obj = json.as<JsonObject>();
//       _obj["type"] = this->_type.c_str();
//       _obj["value"] = this->_value.c_str();
//     }
//   };
// };
// template<typename T>
// NativeData<T>* nativeData(std::string type, T value){
//   return NativeData<T>::create(type, value);
// };
// class ExceptionData: public IResultData{
// private:
//   std::string _type;
//   std::string _message;
//   std::string _details;
// public:
//   ExceptionData(std::string type, std::string message, std::string details)
//   : IResultData("exception"){
//     this->_type = type;
//     this->_message = message;
//     this->_details = details;
//   };
//   virtual void fillJSON(JsonVariant json){
//     if (json.is<JsonObject>()){
//       IResultData::fillJSON(json);
//       JsonObject& _obj = json.as<JsonObject>();
//       _obj["type"] = this->_type.c_str();
//       _obj["message"] = this->_message.c_str();
//       _obj["details"] = this->_details.c_str();
//     }
//   };
//   virtual std::string toJSON(){
//     DynamicJsonBuffer _buffer(512);
//     JsonObject& _obj = _buffer.createObject();
//     this->fillJSON(_obj);
//     return nodeJSON::stringify(_obj);
//   };
// };
// ExceptionData* exceptionData(std::string type, std::string message, std::string details = ""){
//   return new ExceptionData(type, message, details);
// };
// class IResult: public IJSONSupport{
// private:
//   std::string _module;
//   std::string _method;
// protected:
//   IResultData* _data;
//   std::string getUniqueName(){
//     return this->_module + "." + this->_method;
//   };
// public:
//   IResult(std::string module, std::string method){
//     this->_module = module;
//     this->_method = method;
//     this->_data = NULL;
//   };
//   std::string getModule(){ return this->_module; };
//   std::string getMethod(){ return this->_method; };
//   IResult* setData(IResultData* data){
//     if (data != NULL) this->_data = data;
//     return this;
//   };
//   virtual void fillJSON(JsonVariant json){
//     if (json.is<JsonArray>()) this->fillJSON(json.as<JsonArray>().createNestedObject());
//     else if (json.is<JsonObject>()){
//       JsonObject& _data = json.as<JsonObject>().createNestedObject(strdup(this->getUniqueName().c_str()));
//       if (this->_data != NULL) this->_data->fillJSON(_data);
//     };
//   };
//   virtual std::string toJSON(){
//     DynamicJsonBuffer _buffer(512);
//     JsonObject& _obj = _buffer.createObject();
//     this->fillJSON(_obj);
//     return nodeJSON::stringify(_obj);
//   };
// };

/************************************************************************************
 * Module interfaces
 ************************************************************************************/
// class MethodInfo: public IJSONSupport{
//   typedef std::map<std::string, std::string> param_Type;
// private:
//   std::string _module;
//   std::string _moduleName;
//   param_Type* _params;
//   param_Type* getParams(){
//     if (this->_params == NULL) this->_params = new param_Type();
//     return this->_params;
//   };
//   std::string getUniqueName(){
//     return this->_module + "." + this->_moduleName;
//   };
// public:
//   MethodInfo(std::string module, std::string name){
//     this->_module = module;
//     this->_moduleName = name;
//   };
//   std::string getModule(){ return this->_module; };
//   std::string getName(){ return this->_moduleName; };
//   MethodInfo* addParam(std::string name, std::string type){
//     this->getParams()->insert(std::make_pair(name, type));
//     return this;
//   };
//   virtual void fillJSON(JsonVariant json){
//     if (json.is<JsonArray>()) this->fillJSON(json.as<JsonArray>().createNestedObject());
//     else if (json.is<JsonObject>()){
//       JsonArray& _MethodInfo = json.as<JsonObject>().createNestedArray(strdup(this->getUniqueName().c_str()));
//       for (param_Type::iterator _it = this->getParams()->begin(); _it != this->getParams()->end(); ++_it){
//         JsonObject& _param = _MethodInfo.createNestedObject();
//         _param["name"] = _it->first.c_str();
//         _param["type"] = _it->second.c_str();
//       }
//     }
//   };
//   virtual std::string toJSON(){
//     DynamicJsonBuffer _buffer(512);
//     JsonObject& _obj = _buffer.createObject();
//     this->fillJSON(_obj);
//     std::string _json = nodeJSON::stringify(_obj);
//     delete_if_pointer(_obj);
//     return _json;
//   };
// };
// MethodInfo* methodInfo(std::string module, std::string name){
//   return new MethodInfo(module, name);
// };

// typedef std::vector<MethodInfo*> listMethod_Type;
// class IModule{
// private:
//   std::string _moduleName;
//   std::string _moduleVersion;
// protected:
//   typedef IResultData*(IModule::*execHandler)(IExecArgs* args);
//   virtual execHandler getHandler(std::string method){ return NULL; };
//   IModule(std::string name, std::string version = "1.0"){
//     this->_moduleName = name;
//     this->_moduleVersion = version;
//   };
//   virtual IResult* makeResult(std::string method){
//     return (new IResult(this->_moduleName, method));
//   };
// public:
//   IResult* execute(ExecMeta* execInfo){
//     if (execInfo->getModule() != this->_moduleName)
//       return this->makeResult(execInfo->getMethod())->setData(exceptionData("InvalidModule", "Invalid module [" + execInfo->getModule() + "] to execute"));
//     execHandler _handler = this->getHandler(execInfo->getMethod());
//     if (_handler != NULL)
//       return this->makeResult(execInfo->getMethod())->setData((this->*_handler)(execInfo->getArgs()));
//     else
//       return this->makeResult(execInfo->getMethod())->setData(exceptionData("MethodNotFound", "Method [" + execInfo->getMethod() + "] not found"));
//   };
//   virtual listMethod_Type* getMethods(){ return NULL; };
//   virtual bool setup(){ return false; };
//   virtual bool loop(){ return false; };
// };