#include "vnet_client.h"

void vnet::LocalClientConnection::send (const vnet::Destination &dest, const vnet::Message& msg)
{
   this->send (msg, MessageMetadata (id_, dest));
}

void vnet::LocalClientConnection::send (const Message &msg, const MessageMetadata &meta)
{
  downstream()->send (msg, meta);
}

void vnet::LocalClientConnection::received(const vnet::Message& msg, const vnet::MessageMetadata& meta)
{
    const ParcelRef parcel (new Parcel (msg, meta));
    msgs_.push (parcel);
}

vnet::ParcelRef vnet::LocalClientConnection::receive(bool block)
{
    if (block)
        return msgs_.pop_blocking ();
    else {
        bool      empty;
        ParcelRef msg;
        
        msgs_.pop_non_blocking (empty, msg);        
        assert ((empty && (msg == NULL)) || ((!empty) && (msg != NULL)));
        
        return msg;
    }
}
