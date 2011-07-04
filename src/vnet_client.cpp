#include "vnet_client.h"

void vnet::LocalClientConnection::send (const vnet::Destination &dest, const vnet::Message& msg)
{
   this->send (msg, MessageMetadata (id_, dest));
}

void vnet::LocalClientConnection::send (const Message &msg, const MessageMetadata &meta)
{
  downstream()->send (msg, meta);
}