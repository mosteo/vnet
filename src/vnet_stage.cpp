#include "vnet_stage.h"

void vnet::Filter::send(const vnet::Message& msg, const vnet::Envelope& meta)
{
  if (accept_send (msg, meta))
    downstream()->send (msg, meta);
}

void vnet::Filter::received(const vnet::Message& msg, const vnet::Envelope& meta, const NodeId &receiver)
{
  if (accept_receive(msg, meta, receiver))
    upstream()->received(msg, meta, receiver);
}