#include "vnet_localtransport.h"

void vnet::LocalTransport::send(const vnet::Message& msg, const vnet::Envelope& meta)
{

}

void vnet::LocalTransport::received(const vnet::Message& msg, const vnet::Envelope& meta, const vnet::NodeId& receiver)
{
    upstream ()->received(msg, meta, receiver);
}
