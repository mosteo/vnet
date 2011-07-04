#ifndef vnet_localtransport_h_
#define vnet_localtransport_h_

#include "../vnet_core.h"

namespace vnet {
 
  class LocalTransport : public Transport {
  public:
    virtual void send     (const Message &msg, const MessageMetadata &meta) { received (msg, meta); };
    virtual void received (const Message &msg, const MessageMetadata &meta) { upstream ()->received (msg, meta); };
  };
  
}

#endif