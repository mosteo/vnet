#ifndef vnet_message_h_
#define vnet_message_h_

#include <boost/asio/buffer.hpp>
#include <cstring>
#include <vector>
#include <sstream>
#include "vnet_address.h"

namespace vnet {
  
    // The message is an opaque sequence of bytes.
    // It holds a copy of the original data, and as such the sender can dispose of the original data once the message is created.
    // Client libraries might provide some particular adapter in between. 
    class Message {
    public:
        // Raw constructor
        Message (const void *data, const std::size_t size);
                    
        Message (const Message &msg) : data_ (msg.data_) {};
        
        // Helper "constructor" for simple types
        template<typename POD>
        static Message build (const POD &pod) { return Message (static_cast<const void *>(&pod), sizeof (pod)); };  
                
        // Raw accessor for clients
        const void * data () const { return static_cast<const void*>(&data_[0]); };
        
        // Helper for simple types
        template<typename POD>
        const POD &get() const { return *reinterpret_cast<const POD *>(&data_[0]); };
        
        // Raw accessor for transports and perhaps clients
        const boost::asio::const_buffer const_buffer () const { return boost::asio::const_buffer (&data_[0], data_.size ()); };
        
        virtual ~Message () {};
    
    private:
        std::vector<char> data_;
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
        Parcel (const Message &msg, const Envelope &meta) : msg_ (new Message (msg)), meta_ (meta) {};
        
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