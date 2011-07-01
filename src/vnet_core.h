#ifndef vnet_core_h_
#define vnet_core_h_

#include <boost/shared_ptr.hpp>
#include <vector>
#include "vnet_address.h"
#include "vnet_client.h"
#include "vnet_message.h"

namespace vnet {

// A stage is a pluggable network piece (filter, transport).
// In the sender side, the message travels following the Stage.send method.
// In the receiving side, the message travels following the Stage received method.

class Stage {
public:
  Stage () : upstream_ (NULL), downstream_ (NULL) { };
  
  virtual void send     (const Message &msg, const MessageMetadata &meta) = 0;
  virtual void received (const Message &msg, const MessageMetadata &meta) = 0;
  
  virtual ~Stage() { };
  
protected:
  void set_frontend (Stage &upstream);
  void set_backend  (Stage &downstream);
  
private:  
  Stage *upstream_  ;
  Stage *downstream_;
};

// A filter is intended to receive a message and selectively drop it if necessary
class Filter : public Stage {

public:  
  Filter (Stage &backend);
  // To build a chain from downstream towards upstream  
};

// A transport is a special stage in the sense that it is at the end of a stage chain. 
// It is intended to actually pass along over a message to a (possibly) remote endpoint.
class Transport : public Stage {
};

// The network is the frontend for client connections.
class Network : Stage {
public:
  Network (Stage &backend) {};
  // Simple constructor for testing with a single backend
  
  void send (const LocalClientConnection &sender, const Destination dest, const Message &msg);
  
  virtual void send     (const Message &msg, const MessageMetadata &meta) {};
  virtual void received (const Message &msg, const MessageMetadata &meta) {};
  
};

}

#endif