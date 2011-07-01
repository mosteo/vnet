#ifndef vnet_message_h_
#define vnet_message_h_

#include "vnet_address.h"

namespace vnet {
  
  //  For C++ clients we provide a root Message class.
  //  Descendents should override serialize.
  class Message {
  };
  
  //  Addressing of the packet
  class MessageMetadata {
  public:
    MessageMetadata (const NodeId sender, const DestinationRef &receiver);
    
    const NodeId & sender () const;    
    const DestinationRef & receiver () const;
    
  private:
    
    NodeId         sender_;
    DestinationRef receiver_;
    
  };
  
}

#endif