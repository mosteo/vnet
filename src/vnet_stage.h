#ifndef vnet_stage_h_
#define vnet_stage_h_

#include "vnet_message.h"

namespace vnet {

// A stage is a pluggable network piece (filter, transport).
// In the sender side, the message travels following the Stage.send method.
// In the receiving side, the message travels following the Stage received method.

class Stage {
public:
  Stage () : upstream_ (NULL), downstream_ (NULL) { };
  
  virtual void send     (const Message &msg, const MessageMetadata &meta) = 0;
  virtual void received (const Message &msg, const MessageMetadata &meta) = 0;
  
  virtual ~Stage() { };
  
  void set_upstream   (Stage &upstream)   { upstream_   = &upstream;   };
  void set_downstream (Stage &downstream) { downstream_ = &downstream; };

protected:
  Stage *upstream()   { return upstream_;   };
  Stage *downstream() { return downstream_; };
  
private:  
  Stage *upstream_  ;
  Stage *downstream_;
};

// A filter is intended to receive a message and selectively drop it if necessary
class Filter : public Stage {

public:
  virtual void send     (const Message &msg, const MessageMetadata &meta);
  virtual void received (const Message &msg, const MessageMetadata &meta);
  
private:
  virtual bool accept_send    (const Message &msg, const MessageMetadata &meta) = 0;
  // Override this with the actual test against the message when sending
  
  virtual bool accept_receive (const Message &msg, const MessageMetadata &meta) = 0;
  // Override this with the actual test against the message when receiving
  // Usually a test on sending should be enough.
};

// A transport is a special stage in the sense that it is at the end of a stage chain. 
// It is intended to actually pass along over a message to a (possibly) remote endpoint.
class Transport : public Stage {
};

}

#endif