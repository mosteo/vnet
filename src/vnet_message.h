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
    MessageMetadata (const NodeId sender, const Destination & receiver) : sender_ (sender), receiver_ (receiver) {};
    
    const NodeId & sender () const;    
    const Destination & receiver () const;
    
  private:
    
    const NodeId       sender_;
    const Destination &receiver_;
    
  };
  
  class Parcel { 
  public:
    Parcel (const Message &msg, const MessageMetadata &meta) : msg_ (msg), meta_ (meta) {};
    
  private:
    Message         msg_;
    MessageMetadata meta_;
  };
  
}

#endif