#ifndef vnet_core_h_
#define vnet_core_h_

#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <map>
#include "vnet_address.h"
#include "vnet_client.h"
#include "vnet_message.h"

namespace vnet {

// The network is the frontend for local (internal) client connections in the vNet process.
// It basically aggregates all filters and the transport

// This is thread-safe, meaning that as many [id,channel] threads can work concurrently without
// bottlenecking each other (only opening a new connection blocks everybody else).

// For reception, there's a lock at each [id,channel] connection, meaning that again every client
// can block on its queue without impacting other clients.

// At least, this is the theory.

class Network : public Stage {
public:
  Network (const std::string &transport_name);
  // Simple constructor for testing with a single transport
  
  //  Add a filter in the most upstream position
  void push_front (const std::string &filter_name,
                   const boost::program_options::variables_map &vm = boost::program_options::variables_map ());
  
  //  Add a filter in the most downstream position
  void push_back (const std::string &filter_name,
                  const boost::program_options::variables_map &vm = boost::program_options::variables_map ());
  
  void remove_filter (const std::string &filter_name);
  
  LocalClientConnectionRef open (const NodeId &id, const Channel &channel);
  //  Only one client per [id, channel] right now
  
  virtual void send     (const Message &msg, const Envelope &meta);
  virtual void received (const Message &msg, const Envelope &meta, const NodeId &receiver);
  
  virtual std::string name () const { return "core"; };
  
private:    
  // Helper class to index open connections
  class IdChannel {
  public:
    IdChannel (NodeId id, Channel channel) : id_ (id), channel_ (channel) {};
    bool operator< (const IdChannel &r) const {
      return (id_ < r.id_) || ((id_ == r.id_) && (channel_ < r.channel_));
    };
    
    NodeId  id_;
    Channel channel_;
  };
  
  typedef std::map <IdChannel, LocalClientConnectionRef> IdConnectionMap;
  typedef std::pair<IdChannel, LocalClientConnectionRef> IdConnectionElem;
  IdConnectionMap clients_;
  // Collection of all currently connected clients.
  // Also to be able to dispatch to clients on parcel reception
  
  typedef std::map <Channel, NodeGroup> ChannelIdsMap;
  typedef std::pair<Channel, NodeGroup> ChannelIdsElem;
  ChannelIdsMap channel_clients_; 
  // To be able to quickly identify nodes for local broadcast.
  
  mutable boost::shared_mutex clients_mutex_;
  // Shared read access, unique write access to these structures, 
  //   since connection opening should be
  //   much less frequent than data sending/reception.
  
  bool centralized_;
  // Flag indicating we operate in centralized mode.
  // When this is true, broadcast is transformed in multicast to all local clients.
  // Not very happy with this, but in order to keep generality I can't think of anything better,
  //   since actual broadcast mechanisms are Transport-dependent.
  
};

}

#endif