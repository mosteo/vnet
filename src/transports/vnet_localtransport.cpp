#include <boost/foreach.hpp>
#include "vnet_localtransport.h"

const std::string vnet::LocalTransport::name_ = "local";

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
            throw std::runtime_error ("Shouldn't happen with local transport");
            //  Because it has been converted into multicast at Network::send
            break;
    }
}

void vnet::LocalTransport::received(const vnet::Message& msg, const vnet::Envelope& meta, const vnet::NodeId& receiver)
{
    Stage * const next = upstream ();
    
    if (next)
        next->received (msg, meta, receiver);
}
