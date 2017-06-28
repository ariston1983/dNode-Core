#pragma once

#include <string>
#include <vector>
#include "FS.h"
#include "ArduinoJson.h"

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
template<typename T>
bool isNative(){
  return
    isSame<T, bool>::value ||
    isSame<T, char>::value ||
    isSame<T, unsigned char>::value ||
    isSame<T, byte>::value ||
    isSame<T, int16_t>::value ||
    isSame<T, int32_t>::value ||
    isSame<T, int64_t>::value ||
    isSame<T, uint16_t>::value ||
    isSame<T, uint32_t>::value ||
    isSame<T, uint64_t>::value ||
    isSame<T, float>::value ||
    isSame<T, const char*>::value;
};

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
