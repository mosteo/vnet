#include "vnet_core.h"

vnet::Network::Network (Stage &downstream) : Stage () 
{ 
  set_downstream (downstream); 
  downstream.set_upstream (*this);
};

vnet::LocalClientConnectionRef vnet::Network::open(const vnet::NodeId& id)
{
  boost::upgrade_lock<boost::shared_mutex> cli_ro_lock (clients_mutex_);
  
  const IdConnectionMap::iterator conn = clients.find (id);
  
  if (conn != std::map::end) {
    return *conn;
  }
  else {
    boost::upgrade_to_unique_lock<boost::shared_mutex> cli_rw_lock (clients_mutex_);
    LocalClientConnectionRef new_conn (new LocalClientConnection (id, *this));
  }
  
  return LocalClientConnectionRef (new LocalClientConnection (id, *this));
}

void vnet::Network::send (const Message &msg, const MessageMetadata &meta)
{
  downstream()->send (msg, meta);
}

void vnet::Network::received(const vnet::Message& msg, const vnet::MessageMetadata& meta)
{

}
