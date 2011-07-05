#include "vnet_core.h"

vnet::Network::Network (Stage &downstream) : Stage () 
{ 
  set_downstream (downstream); 
  downstream.set_upstream (*this);
};

vnet::LocalClientConnectionRef vnet::Network::open(const NodeId &id, const Channel &channel)
{
  boost::upgrade_lock<boost::shared_mutex> ro_lock (clients_mutex_);
  
  const IdConnectionMap::iterator conn = clients_.find (IdChannel (id, channel));
  
  if (conn != clients_.end()) {
    return conn->second;
  }
  else {
    boost::upgrade_to_unique_lock<boost::shared_mutex> rw_lock (ro_lock);
    LocalClientConnectionRef new_conn (new LocalClientConnection (id, *this));
    clients_.insert (IdConnectionElem (IdChannel (id, channel), new_conn));
    return new_conn;
  }
}

void vnet::Network::send (const Message &msg, const MessageMetadata &meta)
{
  downstream()->send (msg, meta);
}

void vnet::Network::received(const vnet::Message& msg, const vnet::MessageMetadata& meta)
{
    boost::lock_guard<boost::shared_mutex> ro_lock (clients_mutex_);
    
    // const IdConnectionMap::iterator conn = clients_.find (IdChannel (meta.id, meta.receiver().channel));
}
