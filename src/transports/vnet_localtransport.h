#ifndef vnet_localtransport_h_
#define vnet_localtransport_h_

#include "vnet_core.h"
 
namespace vnet {
    
    //  Intended to be used in the centralized vNet setup
 
    class LocalTransport : public Transport {
    public:
        virtual void send     (const Message &msg, const Envelope &meta);
        virtual void received (const Message &msg, const Envelope &meta, const NodeId &receiver);
    };
  
}

#endif