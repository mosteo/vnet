#ifndef vnet_address_h
#define vnet_address_h

#include <set>
#include <string>
#include <boost/shared_ptr.hpp>

namespace vnet {
  
  typedef std::string NodeId;
  //  A networked entity to be referred to by name
  
  typedef std::string ProtocolSpecificAddress;
  //  The particular expression of an address in some protocol

  typedef std::string Channel;
  //  Conceptually equivalent to a port
  
  class Destination {
  public:
    Channel channel () const { return channel_; };
    virtual ~Destination(void) { };
    
  protected:    
    Destination (const Channel &channel) : channel_ (channel) {};
    
  private:
    Channel      channel_;
  };
  
  class UnicastDestination : public Destination {
  public:
    UnicastDestination (const NodeId &id, const Channel &channel) : Destination (channel), id_ (id) {};
    
  private:
    NodeId id_;
  };
  
  typedef std::set<NodeId> NodeGroup;
  
  class MulticastDestination : public Destination {
  public:
    MulticastDestination (const NodeGroup &group, const Channel &channel);
    
  private:
    NodeGroup group_;
  };
  
  class BroadcastDestination : public Destination {
  public:
    BroadcastDestination (const Channel &channel);
  };
  
  // typedef boost::shared_ptr<Destination> DestinationRef;
  // Some of the above
  
  //  An address is an endpoint for a machine in a network.
  //  E.g. the IP:Port combination for TCP/UDP
  class Address {
  public:

  private:
    Address () { }; // Private so no instances
    virtual ~Address () { };
  };

}

#endif