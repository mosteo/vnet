#ifndef vnet_client_h_
#define vnet_client_h_

#include <vector>
#include "vnet_address.h"
#include "vnet_stage.h"
#include "vnet_message.h"

namespace vnet {
  
  // Server side of a client connection.
  // That is, this is internal to the vnet process.
  class LocalClientConnection : public Stage {
  public:
    LocalClientConnection (const NodeId &id, Stage &downstream) : Stage (), id_ (id) { set_downstream (downstream); };
    
    void send (const Destination &dest, const Message &msg);
    
    const NodeId & node_id () const { return id_; } ;        
    
    void dispatcher();
    //  To be started in a new thread upon connection creation
    //  This thread calls the client for any newly received msg.
    
    virtual ~LocalClientConnection () {};
    
  private:
    virtual void send     (const Message &msg, const MessageMetadata &meta);  
    virtual void received (const Message &msg, const MessageMetadata &meta);
    
    NodeId id_;
    
    std::vector<Parcel> msgs;
  };

typedef boost::shared_ptr<LocalClientConnection> LocalClientConnectionRef;

}

#endif