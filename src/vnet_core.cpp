#include <boost/cast.hpp>
#include "vnet_core.h"
#include <boost/thread/locks.hpp>

vnet::Network::Network (const std::string &transport_name) : Stage (), centralized_ (true) 
{ 
  Transport *transport = boost::polymorphic_downcast<Transport *>(StageFactory::create(transport_name));    
  set_downstream (transport); 
  transport->set_upstream (this);
};

void vnet::Network::push_front (const std::string &filter_name)
{
    Filter * filter = boost::polymorphic_downcast<Filter *>(StageFactory::create(filter_name));
 
    //  Prevent several stage modifications simultaneously
    boost::unique_lock<boost::shared_mutex> lock (clients_mutex_);
    
    filter->set_downstream (downstream ());
    filter->set_upstream   (this);
    //  Now the new filter is ready to take over delivery.
    
    downstream ()->set_upstream (filter);
    //  Inbound packets already use the new filter, but outbound not yet.
    
    set_downstream (filter);
    //  Restructuring is complete.
    
    //  Given that some filters may rely on being present in both endpoints
    //    (e.g. a compressing/uncompressing filter),
    //    these should be set-up off-line.
};

void vnet::Network::push_back (const std::string &filter_name)
{
    Filter * filter = boost::polymorphic_downcast<Filter *>(StageFactory::create(filter_name));
    
    //  Prevent several stage modifications simultaneously
    boost::unique_lock<boost::shared_mutex> lock (clients_mutex_);
    
    Stage *last = downstream();
    
    //  skip to last stage (i.e. the transport)
    while (last->downstream() != NULL)
        last = last->downstream();
    
    filter->set_downstream (last);
    filter->set_upstream   (last->upstream());
    //  Now the new filter is ready to take over delivery.
    
    last->set_upstream (filter);
    //  Inbound packets already use the new filter, but outbound not yet.
    
    filter->upstream()->set_downstream (filter);
    //  Restructuring is complete.
    
    //  Given that some filters may rely on being present in both endpoints
    //    (e.g. a compressing/uncompressing filter),
    //    these should be set-up off-line.
};

void vnet::Network::remove_filter (const std::string &filter_name)
{
    //  Prevent several stage modifications simultaneously
    boost::unique_lock<boost::shared_mutex> lock (clients_mutex_);
    
    Stage *target = NULL;
    
    for (Stage *s = downstream(); s->downstream() != NULL; s = s->downstream()) {
        if (s->name() == filter_name) {
            target = s;
            break;
        }
    }
    
    if (!target)
        throw std::runtime_error ("remove_filter: " + filter_name + "not found");
    
    target->upstream()->set_downstream(target->downstream());
    target->downstream()->set_upstream(target->upstream());
    
    // target is out of chain, but there could be packets going through still!
    // So this should fail in the tests:
    delete target;
};

vnet::LocalClientConnectionRef vnet::Network::open(const NodeId &id, const Channel &channel)
{
  boost::upgrade_lock<boost::shared_mutex> ro_lock (clients_mutex_);
  
  const IdConnectionMap::iterator conn = clients_.find (IdChannel (id, channel));
  
  if (conn != clients_.end()) {
    return conn->second;
    //  Here we are returning the same handle even if it's a re-open.
    //  This means that two clients with same id could send via the same channel,
    //  but would steal packets on reception from each other non-deterministically.
    //  TODO: Fail here or remove the unique [id,channel] restriction.
    //  TODO: Implement connection closing (and timeout?).
  }
  else {
    boost::upgrade_to_unique_lock<boost::shared_mutex> rw_lock (ro_lock);
    
    // Add the connection to the list.
    LocalClientConnectionRef new_conn (new LocalClientConnection (id, this));
    clients_.insert (IdConnectionElem (IdChannel (id, channel), new_conn));
    
    // Add the client id to the channel list.
    const ChannelIdsMap::iterator clients_in_channel = channel_clients_.find (channel);
    if (clients_in_channel != channel_clients_.end ())
        clients_in_channel->second.insert (id);
    else {
        NodeGroup new_group;
        new_group.insert (id);
        channel_clients_.insert (ChannelIdsElem (channel, new_group));
    }
    
    return new_conn;
  }
}

void vnet::Network::send (const Message &msg, const Envelope &meta)
{
    //  We don't check downstream() != NULL below because it should be valid after Network construction
    //  Changes to its value are thread-safe too.
    
    if (centralized_ && meta.receiver ().kind () == Destination::broadcast) {
        NodeGroup new_clients;
        {
            //  We pass along the current clients in the given channel to be able to perform local broadcast at the transport end
            boost::upgrade_lock<boost::shared_mutex> ro_lock (clients_mutex_);
            //  Read lock since a copy of the receivers is going to be made.
            //  The following look-up cannot fail; at least the sender is in the channel:
            new_clients = channel_clients_.find (meta.receiver ().channel ())->second;
        }
        
        //  The actual send to downstream is thread-safe since we have a copy of receivers now.
        downstream ()->send (msg,
                             Envelope (meta.sender (), 
                                       Destination (new_clients, meta.receiver ().channel ())));
    }
    else // Just pass along
        downstream ()->send (msg, meta);
}

void vnet::Network::received(const vnet::Message& msg, const vnet::Envelope& meta, const NodeId &receiver)
{
    LocalClientConnectionRef client;
    
    //  Thread-safely get a reference to the receiving client
    {
        boost::lock_guard<boost::shared_mutex> ro_lock (clients_mutex_);    
        const IdConnectionMap::iterator conn = clients_.find (IdChannel (receiver, meta.receiver ().channel ()));
        if (conn != clients_.end ())
            client = conn->second;
    }
    
    if (client != NULL)
        client->received(msg, meta, receiver);
    //  TODO: log message dropped here
    //  Otherwise we should log failure I guess.
}
