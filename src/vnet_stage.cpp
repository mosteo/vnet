#include "vnet_stage.h"

void vnet::Filter::send(const vnet::Message& msg, const vnet::Envelope& meta)
{
    Stage * const next = downstream ();
    
    if (next != NULL)
        if (accept_send (msg, meta))
            next->send (msg, meta);
}

void vnet::Filter::received(const vnet::Message& msg, const vnet::Envelope& meta, const NodeId &receiver)
{
    Stage * const next = upstream ();
    
    if (next != NULL)
        if (accept_receive(msg, meta, receiver))
            next->received(msg, meta, receiver);
}