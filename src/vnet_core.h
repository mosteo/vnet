#ifndef vnet_core_h_
#define vnet_core_h_

#include "vnet_address.h"

namespace vnet {

// A stage is a pluggable network piece (filter, transport).
class Stage {
  
public:
  virtual void Send (const Destination &dest) = 0;
  
  virtual ~Stage(void) = 0;
  
private:
  static Stage * Create(const StageConfig &config) = 0;
  //  Creation procedure that should perform any necessary configuration on this stage.
  
};

// A filter is intended to receive a message and selectively drop it if necessary
class Filter : Stage {

public:  
  void SetBackend (Stage *backend);

private:
  Stage *backend_; // Where to pass along a message
  
};

// A transport is a special stage in the sense that it is at the end of a stage chain. 
// It is intended to actually pass along over a message to a (possibly) remote endpoint.
class Transport : Stage {
};

}

#endif