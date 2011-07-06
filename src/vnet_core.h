#ifndef vnet_core_h_
#define vnet_core_h_

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <map>
#include "vnet_address.h"
#include "vnet_client.h"
#include "vnet_message.h"

namespace vnet {

// The network is the frontend for client connections.
class Network : public Stage {
public:
  Network (Transport &downstream);
  // Simple constructor for testing with a single transport
  
  LocalClientConnectionRef open (const NodeId &id, const Channel &channel);
  //  Only one client per [id, channel] right now
  
  virtual void send     (const Message &msg, const Envelope &meta);
  virtual void received (const Message &msg, const Envelope &meta, const NodeId &receiver);
  
private:
  
  class IdChannel {
  public:
    IdChannel (NodeId id, Channel channel) : id_ (id), channel_ (channel) {};
    bool operator< (const IdChannel &r) const {
      return (id_ < r.id_) || ((id_ == r.id_) && (channel_ < r.channel_));
    };
    
    NodeId  id_;
    Channel channel_;
  };
  
  typedef std::map<IdChannel, LocalClientConnectionRef>  IdConnectionMap;
  typedef std::pair<IdChannel, LocalClientConnectionRef> IdConnectionElem;
  IdConnectionMap clients_;
  mutable boost::shared_mutex clients_mutex_;
  // The collection of clients to dispatch messages to.
  // Shared read access, unique write access
  
  Transport *transport_;
  // In order to be able to call prepare_broadcast through our Addressbook view 
  
};

}

#endif