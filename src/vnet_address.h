#ifndef vnet_address_h
#define vnet_address_h

#include <set>
#include <string>
#include <boost/shared_ptr.hpp>

namespace vnet {
  
  typedef std::string NodeId;
  //  A networked entity to be referred to by name
  
  typedef std::set<NodeId> NodeGroup;
  //  For multicast. A set is less efficient but simplifies right now the elimination of duplicate receivers.

  typedef std::string Channel;
  //  Conceptually equivalent to a port    
    
  //  Not sure yet on how to deal with all central/distrib x transport x multi/broadcast combinations, I need to discuss.
  //  For the moment this uses a "variant" approach which is not well supported by c++, but...
  class Destination {    
  public:
    enum Kinds { unicast, multicast, broadcast };
    
    Destination (const NodeId    &dest, const Channel &channel) : kind_ (unicast),   id_    (dest), channel_ (channel) {};
    Destination (const NodeGroup &dest, const Channel &channel) : kind_ (multicast), group_ (dest), channel_ (channel) {};
    Destination (                       const Channel &channel) : kind_ (broadcast),                channel_ (channel) {};
    
    Kinds             kind ()    const { return kind_;    };
    NodeId            id ()      const { return id_;      };
    const NodeGroup & group ()   const { return group_;   };
    Channel           channel () const { return channel_; };
    
    virtual ~Destination(void) {};
    // Although this class is intended to be final!!
        
  private:
    Kinds           kind_;
    NodeId          id_;      // Only valid for unicast
    NodeGroup       group_;   // Only valid for multicast/broadcast
    Channel         channel_;
  };
  
  typedef boost::shared_ptr<Destination> DestinationRef;

}

#endif