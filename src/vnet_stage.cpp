#include "vnet_stage.h"

using namespace vnet;

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

std::map<const std::string, vnet::StageFactory::StageConstructor> vnet::StageFactory::factory_;

Stage * vnet::StageFactory::create (const std::string name, 
                                    const boost::program_options::variables_map &vm) 
{ 
    if (factory_.find (name) != factory_.end ())
        return factory_[name](vm); 
    else 
        throw std::runtime_error (name + ": Stage not registered in factory.");
}