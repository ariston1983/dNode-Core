#pragma once

#include "Arduino.h"
#include <string>
#include <vector>
#include "FS.h"
#include "ArduinoJson.h"

namespace dNode{
  /******************************************************************************
  * Pointer helper
  ******************************************************************************/
  template<typename T> inline
  void delete_if_pointer(T const&){ };
  template<typename T> inline
  void delete_if_pointer(T* const& p){ delete p; };

  /******************************************************************************
  * Type helper
  ******************************************************************************/
  template<typename T, typename U>
  struct isSame{ static const bool value = false; };
  template<typename T>
  struct isSame<T, T>{ static const bool value = true; };
  template<bool condition, typename T = void>
  struct enableIf{ };
  template<typename T>
  struct enableIf<true, T>{ typedef T type; };
  
  template<typename T>
  struct pointer_template{ static const bool value = false; };
  template<typename T>
  struct pointer_template<T*>{ static const bool value = true; };
  template<typename T>
  struct isPointer{ static const bool value = pointer_template<T>::value; };
  template<typename T>
  struct clearPointer{ typedef T type; };
  template<typename T>
  struct clearPointer<T*>{typedef T type; };

  template<typename T>
  struct reference_template{ static const bool value = false; };
  template<typename T>
  struct reference_template<T&>{ static const bool value = true; };
  template<typename T>
  struct isReference{ static const bool value = reference_template<T>::value; };
  template<typename T>
  struct clearReference{ typedef T type; };
  template<typename T>
  struct clearReference<T&>{ typedef T type; };

  template<typename T>
  struct const_template{ static const bool value = false; };
  template<typename T>
  struct const_template<const T>{ static const bool value = true; };
  template<typename T>
  struct isConst{ static const bool value = const_template<T>::value; };
  template<typename T>
  struct clearConst{ typedef T type; };
  template<typename T>
  struct clearConst<const T>{ typedef T type; };

  template<typename T>
  struct clearType{ typedef T type; };
  template<typename T>
  struct clearType<T&>{ typedef T type; };
  template<typename T>
  struct clearType<T*>{ typedef T type; };
  template<typename T>
  struct clearType<const T>{ typedef T type; };
  template<typename T>
  struct clearType<const T&>{ typedef T type; };
  template<typename T>
  struct clearType<const T*>{ typedef T type; };

  template<class T>
  struct clearClass{ typedef T type; };
  template<class T>
  struct clearClass<T&>{ typedef T type; };
  template<class T>
  struct clearClass<T*>{ typedef T type; };
  template<class T>
  struct clearClass<const T>{ typedef T type; };
  template<class T>
  struct clearClass<const T&>{ typedef T type; };
  template<class T>
  struct clearClass<const T*>{ typedef T type; };

  template<class TBase, class TDerived>
  struct pre_based_of{
  protected:
    typedef char Yes[1];
    typedef char No[2];
    static Yes &probe(const TBase*);
    static No &probe(...);
  public:
    enum{
      value = sizeof(probe(reinterpret_cast<TDerived*>(0))) == sizeof(Yes)
    };
  };
  template<class TBase, class TDerived>
  struct isBaseOf: pre_based_of<typename clearClass<TBase>::type, typename clearClass<TDerived>::type>{ };

  template<typename T, typename U>
  typename enableIf<isBaseOf<T, U>::value || isSame<T, U>::value, bool>::type isValueOf(U* value){ return true; };
  template<typename T, typename U>
  typename enableIf<!isBaseOf<T, U>::value && !isSame<T, U>::value, bool>::type isValueOf(U* value){ return false; };

  class Variant;
  template<typename T>
  struct isVariant{ static const bool value = isBaseOf<dNode::Variant, typename clearClass<T>::type>::value; };

  template<typename T>
  struct isBool{ static const bool value = isSame<bool, T>::value; };
  template<typename T>
  struct isInteger{
    static const bool value =
      isSame<bool, T>::value ||
      isSame<signed char, T>::value ||
      isSame<unsigned char, T>::value ||
      isSame<signed short, T>::value ||
      isSame<unsigned short, T>::value ||
      isSame<signed int, T>::value ||
      isSame<unsigned int, T>::value ||
      isSame<signed long, T>::value ||
      isSame<unsigned long, T>::value;
  };
  template<typename T>
  struct isFloat{
    static const bool value = isSame<float, T>::value || isSame<double, T>::value;
  };
  template<typename T>
  struct isChars{ static const bool value = isSame<char, T>::value || isSame<const char*, T>::value; };
  template<typename T>
  struct isString{ static const bool value = isSame<std::string, T>::value; };
  template<typename T>
  struct isNative{
    static const bool value = isBool<T>::value ||
      isInteger<T>::value ||
      isFloat<T>::value ||
      isChars<T>::value ||
      isString<T>::value;
  };

  template<typename T>
  const T& val(const T& value, typename enableIf<!isChars<T>::value, T>::type* = 0){ return value; };
  template<typename T>
  const T* val(const T* value, typename enableIf<isChars<T>::value, T>::type* = 0){ return value; };
  template<typename T>
  T& val(T* value, typename enableIf<!isChars<T>::value, typename clearType<T>::type>::type* = 0){ return *value; };

  template<typename T>
  const T* ptr(const T& value, typename enableIf<isNative<T>::value && !isChars<T>::value, T>::type* = 0){ return &value; };
  template<typename T>
  const T* ptr(const T* value, typename enableIf<isChars<T>::value, T>::type* = 0){ return value; };
  template<typename T>
  T* ptr(T& value, typename enableIf<!isChars<T>::value, typename clearType<T>::type>::type* = 0){ return &value; };
  template<typename T>
  T* ptr(T* value, typename enableIf<!isChars<T>::value, typename clearType<T>::type>::type* = 0){ return value; };
};

bool isJSONBool(JsonVariant json){ return json.is<bool>(); };
bool isJSONInteger(JsonVariant json){
  return json.is<signed char>() ||
    json.is<unsigned char>() ||
    json.is<signed short>() ||
    json.is<unsigned short>() ||
    json.is<signed int>() ||
    json.is<unsigned int>() ||
    json.is<signed long>() ||
    json.is<unsigned long>();
};
bool isJSONFloat(JsonVariant json){ return json.is<float>(); };
bool isJSONChars(JsonVariant json){ return json.is<const char*>(); };

/******************************************************************************
 * JSON object modifier
 ******************************************************************************/
class nodeJSON{
public:
  static std::string stringify(JsonVariant json){
    int _len = json.measureLength()+1;
    char _store[_len];
    json.printTo(_store, _len);
    return std::string(_store);
  };
};
/* End of JSON modifier */

/******************************************************************************
 * File system modifier
 ******************************************************************************/
class nodeFile{
  class FileInfo{
  private:
    std::string _filename;
    int _size;
  public:
    static FileInfo* create(std::string filename, int size){ return new FileInfo(filename, size); }
    FileInfo(std::string filename, int size){
      this->_filename = filename;
      this->_size = size;
    };
    std::string getFilename(){ return this->_filename; };
    int getSize(){ return this->_size; };
  };
  typedef std::vector<FileInfo*> dirList_Type;
public:
  static bool init(bool format = false){
    SPIFFS.begin();
    if (!SPIFFS.exists("/fs.txt") || format == true){
      SPIFFS.format();
      File _fs = SPIFFS.open("/fs.txt", "w");
      if (_fs){
        _fs.println("## This is just header file");
        _fs.close();
      }
      else return false;
    }
    return true;
  };
  static dirList_Type* dir(std::string root = "/"){
    dirList_Type* _files = new dirList_Type();
    if (init()){
      Dir _dir = SPIFFS.openDir(root.c_str());
      while (_dir.next()){
        File _file = _dir.openFile("r");
        _files->push_back(FileInfo::create(_dir.fileName().c_str(), _file.size()));
        _file.close();
      }
    }
    SPIFFS.end();
    return _files;
  };
  static bool exists(std::string file, std::string dir = "/"){
    bool _res = false;
    if (init()){
      std::string _path = dir + file;
      _res = SPIFFS.exists(_path.c_str());
    }
    SPIFFS.end();
    return _res;
  };
  static std::string readFile(std::string file, std::string dir = "/"){
    std::string _content = "";
    if (exists(file, dir)){
      std::string _path = dir + file;
      File _file = SPIFFS.open(_path.c_str(), "r");
      _content = _file.readString().c_str();
      _file.close();
    }
    SPIFFS.end();
    return _content;
  };
  static bool writeFile(std::string content, std::string file, std::string dir = "/"){
    bool _res = false;
    if (init()){
      std::string _path = dir + file;
      File _file = SPIFFS.open(_path.c_str(), "w");
      _res = _file.println(content.c_str());
      _file.close();
    }
    SPIFFS.end();
    return _res;
  };
  static bool removeFile(std::string file, std::string dir = "/"){
    bool _res = false;
    if (exists(file, dir)){
      std::string _path = dir + file;
      _res = SPIFFS.remove(_path.c_str());
    }
    SPIFFS.end();
    return _res;
  };
};
/* End of File system modifier */
