#ifndef vnet_message_h_
#define vnet_message_h_

#include "vnet_address.h"

namespace vnet {
  
  //  For local C++ clients we provide a root Message class.
  //  The Clone is needed because a copy is buffered at the client connection frontend.
  //  For future interfaces some kind of serialization would be necessary.  
  class Message {
  public:
      virtual Message * clone () const = 0;
  };
  
  //  Addressing metadata of the packet
  class Envelope {
  public:
    Envelope (const NodeId & sender, const Destination & receiver) : sender_ (sender), receiver_ (receiver) {};
    
    const NodeId      & sender   () const { return sender_; };    
    const Destination & receiver () const { return receiver_; };
    
  private:    
    const NodeId      sender_;
    const Destination receiver_;    
  };
  
  // A Parcel aggregates a message with its envelope. Used to simplify internal queues and delivery to client.
  class Parcel { 
  public:
    Parcel (const Message &msg, const Envelope &meta) : msg_ (msg.clone ()), meta_ (meta) {};
    
    const Message  &message  () const { return *msg_; };
    const Envelope &envelope () const { return  meta_; };
    
  private:
    // This is the first place where the message is copied
    boost::shared_ptr<Message>  msg_;
    Envelope                    meta_;
  };
  
  typedef boost::shared_ptr<Parcel> ParcelRef; 
  // Premature optimization, most likely, but also useful since the client will keep a ref while the queued one can be dropped.
  
}

#endif