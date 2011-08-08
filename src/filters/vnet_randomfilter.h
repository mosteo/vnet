#ifndef VNET_VNET_RANDOMFILTER_H
#define VNET_VNET_RANDOMFILTER_H

#include "boost/random.hpp"
#include "vnet_stage.h"

namespace vnet {

class RandomFilter : public Filter
{
public:
    RandomFilter (double pdrop = 0.0, uint32_t seed = 0) :
    // Probability of dropping a packet, per byte of length
        pdrop_ (pdrop), 
        generator_ (seed), 
        uniform_dist_ (0.0, 1.0),
        rnd_ (generator_, uniform_dist_) { };    
        
    static const std::string name_;
    virtual std::string name () const { return name_; };
    
protected:
    virtual bool accept_send    (const Message &msg, const Envelope &meta) const { return true; };
    // Always pass on sending.
    
    virtual bool accept_receive (const Message &msg, const Envelope &meta, const NodeId &receiver) const;
    
private:
    double pdrop_;
    
    boost::mt19937 generator_;
    boost::uniform_real<> uniform_dist_;
    mutable boost::variate_generator<boost::mt19937 &, boost::uniform_real<> > rnd_;//(generator_, uniform_dist_);
};

}

#endif // VNET_VNET_RANDOMFILTER_H
