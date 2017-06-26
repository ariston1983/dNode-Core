#pragma once

#include "rpc-Base.hpp"

IRPCResponse* rpc_OnReceived_EventHandler(IRPCBase* context, IRPCRequest* request){
  return context->exec(request);
};

template<class TComm>
class nodeRPC: public IRPCBase{
private:
  IRPCComm* _comm;
public:
  nodeRPC(TComm* comm){
    if (comm != NULL){
      this->_comm = static_cast<IRPCComm*>(comm);
      this->_comm->setRPCEngine(this);
      this->_comm->event_OnReceived(&rpc_OnReceived_EventHandler);
    }
  };
};