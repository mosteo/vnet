#include <boost/foreach.hpp>
#include "vnet_localtransport.h"

void vnet::LocalTransport::send (const vnet::Message& msg, const vnet::Envelope& meta)
{
    switch (meta.receiver ().kind ()) {    
        case Destination::unicast:
            received (msg, meta, meta.receiver ().id ());
            break;
            
        case Destination::multicast:
            BOOST_FOREACH (NodeId receiver, meta.receiver ().group ())
            {
                received (msg, meta, receiver);
                // We could directly call upstream and save two calls,
                //  but just in case at some point we need to do something at this->received()...
            }
            break;
            
        case Destination::broadcast:
            throw "Shouldn't happen with local transport";
            //  Because it has been converted into multicast at Network::send
            break;
    }
}

void vnet::LocalTransport::received(const vnet::Message& msg, const vnet::Envelope& meta, const vnet::NodeId& receiver)
{
    upstream ()->received (msg, meta, receiver);
}
