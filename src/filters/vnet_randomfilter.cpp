#include <iostream>
#include "vnet_randomfilter.h"

using namespace vnet;

const std::string vnet::RandomFilter::name_ = "random drop";

bool RandomFilter::accept_receive(const vnet::Message& msg, const vnet::Envelope& meta, const vnet::NodeId& receiver) const
{
    const double p_keep_byte = 1.0 - pdrop_;
    const double p_keep_all  = pow(p_keep_byte, sizeof (msg));
    const double u           = rnd_ ();
    
    {
        using namespace std;
        cout << (p_keep_all >= u ? "Passing" : "Dropping") << " msg: " << p_keep_all << " ~ " << u << endl;
    }
    
    return p_keep_all >= u;
}


