#ifndef vnet_parcel_h_
#define vnet_parcel_h_

#include "vnet_message.h"

namespace vnet {
  
  typedef boost::shared_array<char> RawParcel;
  //  The thing that comes from the wire

  // A parcel is the internal view of raw data plus metadata,
  //   contrarily to a message which is the client view
  class Parcel {
  public:
    Parcel (const Message &msg, const NodeId &sender, const DestinationRef &receiver);
    // Constructor for the sender side
    
    Parcel (const RawParcel &parcel);
    // Constructor for the receiver side
    
    Message & message () const;
    // Retrieve the message
    
    NodeId & sender () const;
    // Retrieve the sender
    
    DestinationRef receiver () const;
    // Retrieve the intended recipient
    
  };

}

#endif