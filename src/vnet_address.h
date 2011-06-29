#ifndef vnet_address_h
#define vnet_address_h

#include <string>

namespace vnet {
  
  typedef std::string NodeId;
  //  A networked entity to be referred to by name
  
  typedef std::string ProtocolSpecificAddress;
  //  The particular expression of an address in some protocol

  typedef std::string Channel;
  //  Conceptually equivalent to a port
  
  class Destination {
    virtual ~Destination(void) = 0;
  };
  
  class UnicastDestination : Destination {
  };
  
  class MulticastDestination : Destination {
  };
  
  class BroadcastDestination : Destination {
  };
  
  //  An address is an identifier for a machine in a network.
  //  E.g. the IP host for TCP/UDP
  class AbstractAddress {
  public:
    virtual ~AbstractAddress(void) = 0;
  };

}

#endif