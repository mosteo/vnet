#include <boost/array.hpp>
#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <exception>
#include <iostream>

#include "vnet.h"
#include "../filters/vnet_randomfilter.h"
#include "../filters/vnet_filters.h"
#include "../transports/vnet_localtransport.h"
#include "../transports/vnet_transports.h"

using namespace vnet;

//  This basic example creates the vnet and the clients in a single process.
//  Two example clients are created as new threads.
//  Local delivery and client connections are shown.

//  An example filter is constantly added and removed in order to test online reconfiguration

LocalTransport local_transport;
RandomFilter   rnd_filter (0.05, 0);
Network        *net;

boost::array<NodeId, 2> sender_ids   = {{"Ari", "Ben"}};
const NodeId receiver_id = "Zak";

const Channel channel = "monotest";

void sender (int index) {
  
  try {
      
    const NodeId sender_id = sender_ids.at (index);
  
    std::cout << "Sender " << sender_id << " starting..." << std::endl;
    
    LocalClientConnectionRef conn = net->open (sender_id, channel);
    
    boost::mt19937 gen (index);
    boost::uniform_int<> dist (50, 99);
    boost::variate_generator<boost::mt19937 &, boost::uniform_int<> > rnd (gen, dist);
    
    while (true) {
        const int pause = rnd();
        
        std::cout << "I (" << sender_id << ") shall sleep for " << pause << " ms." << std::endl;
        
        boost::this_thread::sleep
          (boost::posix_time::milliseconds
            (pause));
        
        conn->send (receiver_id, channel, Message::build<int>(pause));
    }
    
  } 
  catch (std::exception &e) {
    std::cout << "Exception [sender]: " << e.what () << std::endl;
  }
  catch (...) {
    std::cout << "Unknown exception." << std::endl;
  }
}

void receiver () {
    try {
        
        std::cout << "Receiver starting..." << std::endl;
        
        LocalClientConnectionRef conn = net->open (receiver_id, channel);
        
        while (true) {            
            const ParcelRef parcel = conn->receive ();
            std::cout << "Sender " << 
                parcel->envelope ().sender ()
                << " slept " << 
                parcel->message ().get<int> ()
                << " milliseconds" << std::endl;
        }
        
    } 
    catch (std::exception &e) {
        std::cout << "Exception [receiver]: " << e.what () << std::endl;
    }
    catch (...) {
        std::cout << "Unknown exception." << std::endl;
    }
}

int main(int argc, char **argv) {
    
    vnet::filter::register_all();
    vnet::transport::register_all ();      
    
    net = new Network (LocalTransport::name_);
  
    net->push_front (RandomFilter::name_);
    
    std::cout << "vnet-filter-local started, accepting connections..." << std::endl;
    
    boost::thread sender1   (sender, 0);
    boost::thread sender2   (sender, 1);
    boost::thread receiver1 (receiver);          
    
    // Insert and remove the filter every 20ms
    while (true) {
        net->remove_filter (RandomFilter::name_);
        boost::this_thread::sleep (boost::posix_time::milliseconds (20));        
        net->push_front (RandomFilter::name_);
        boost::this_thread::sleep (boost::posix_time::milliseconds (20));  
        net->remove_filter (RandomFilter::name_);
        boost::this_thread::sleep (boost::posix_time::milliseconds (20));        
        net->push_back (RandomFilter::name_);
        boost::this_thread::sleep (boost::posix_time::milliseconds (20));
    }
    
    sender1.join();
    sender2.join();
    receiver1.join();
    
    return 0;
}