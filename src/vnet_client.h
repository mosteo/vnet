#ifndef vnet_client_h_
#define vnet_client_h_

#include "vnet_address.h"

namespace vnet {
  
  // Server side of a client connection.
  // That is, this is internal to the vnet process.
  class LocalClientConnection {
  public:
    LocalClientConnection (const NodeId &id);
    
    const NodeId & node_id () const; 
    
  private:
    NodeId id_;
    
    virtual ~LocalClientConnection () { };
  };

}

#endif