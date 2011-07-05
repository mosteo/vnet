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
    
    const NodeId      & sender   () const { return sender_; };    
    const Destination & receiver () const { return receiver_; };
    
  private:
    
    const NodeId       sender_;
    const Destination &receiver_;
    
  };
  
  class Parcel { 
  public:
    Parcel (const Message &msg, const MessageMetadata &meta) : msg_ (new Message (msg)), meta_ (new MessageMetadata (meta)) {};
    
    const Message         &message  () const { return *msg_; };
    const MessageMetadata &metadata () const { return *meta_; };
    
  private:
    // This is the first place where the message is copied, since there can be arbitrary time between delivery and processing
    boost::shared_ptr<Message>         msg_;
    boost::shared_ptr<MessageMetadata> meta_;
  };
  
  typedef boost::shared_ptr<Parcel> ParcelRef; 
  // Premature optimization most likely but also useful since the client will keep a ref while the queued one is dropped
  
}

#endif