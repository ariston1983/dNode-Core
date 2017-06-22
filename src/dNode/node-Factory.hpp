#pragma once

#include <string>
#include "node-Module.hpp"
#include "node-Query.hpp"
#include "node-Response.hpp"

template <typename TMod>
TMod* createModule(){ return new TMod(); };

class nodeFactory{
  typedef std::map<std::string, nodeModule*(*)()> modList_Type;
private:
  static modList_Type* _modList;
  static modList_Type* getList(){
    if (_modList == NULL) _modList = new modList_Type();
    return _modList;
  };
public:
  template <typename TRegMod>
  static void reg(std::string const& mod){
    getList()->insert(std::make_pair(mod, &createModule<TRegMod>));
  };
  static nodeModule* create(std::string mod){
    modList_Type::iterator _it = getList()->find(mod);
    if (_it == getList()->end()) return NULL;
    else return _it->second();
  };
  static bool setup(bool ignoreError = false){
    for (modList_Type::iterator _it = getList()->begin(); _it != getList()->end(); ++_it){
      nodeModule* _mod = _it->second();
      if (!_mod->setup() && !ignoreError) return false;
    }
    return true;
  };
  static bool loop(bool ignoreError = false){
    for (modList_Type::iterator _it = getList()->begin(); _it != getList()->end(); ++_it){
      nodeModule* _mod = _it->second();
      if (!_mod->loop() && !ignoreError) return false;
    }
    return true;
  };
  static nodeResponse* exec(nodeQuery* query){
    nodeModule* _mod = create(query->getModule());
    if (_mod == NULL) return NULL;
    else return _mod->exec(query);
  };
};
