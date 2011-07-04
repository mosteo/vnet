#include "vnet_stage.h"

void vnet::Filter::send(const vnet::Message& msg, const vnet::MessageMetadata& meta)
{
  if (accept_send (msg, meta))
    downstream()->send (msg, meta);
}

void vnet::Filter::received(const vnet::Message& msg, const vnet::MessageMetadata& meta)
{
  if (accept_receive(msg, meta))
    upstream()->received(msg, meta);
}