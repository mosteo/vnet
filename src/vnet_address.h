#ifndef vnet_address_h
#define vnet_address_h

#include <list>
#include <string>
#include <boost/shared_ptr.hpp>

namespace vnet {
  
  typedef std::string NodeId;
  //  A networked entity to be referred to by name
  
  typedef std::list<NodeId> NodeGroup;
  //  For multicast
  
  typedef std::string ProtocolSpecificAddress;
  //  The particular expression of an address in some protocol

  typedef std::string Channel;
  //  Conceptually equivalent to a port    
    
  //  Not sure yet on how to deal with all central/distrib x transport x multi/broadcast combinations, I need to discuss.
  class Destination {    
  public:
    enum Kinds { unicast, multicast, broadcast };
    Destination (const NodeId    &dest, const Channel &channel) : kind_ (unicast),   id_    (dest), channel_ (channel) {};
    Destination (const NodeGroup &dest, const Channel &channel) : kind_ (multicast), group_ (dest), channel_ (channel) {};
    Destination (                       const Channel &channel) : kind_ (broadcast),                channel_ (channel) {};
      
    Kinds             kind ()    const { return kind_; };
    NodeId            id ()      const { return id_; };
    const NodeGroup & group ()   const { return group_; };
    Channel           channel () const { return channel_; };
    
    virtual ~Destination(void) { };
        
  private:
    Kinds           kind_;
    NodeId          id_;      // Only valid for unicast
    NodeGroup       group_;   // Only valid for multicast
    Channel         channel_;
  };
  
  typedef boost::shared_ptr<Destination> DestinationRef;
  
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