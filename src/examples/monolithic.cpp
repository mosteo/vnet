#include <boost/array.hpp>
#include <boost/cast.hpp>
#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <exception>
#include <iostream>

#include "../vnet.h"
#include "../transports/vnet_localtransport.h"

using namespace vnet;

//  This basic example creates the vnet and the clients in a single process.
//  Two example clients are created as new threads.
//  Local delivery and client connections are shown.

LocalTransport local_transport;
Network net (local_transport);

class IntMessage : public Message {
public:
  IntMessage (int i) { i_ = i; };
  
  int get() const { return i_; };
  
private:
  int i_;
};

const NodeId sender_id   = "Ari";
const NodeId receiver_id = "Ben";

const Channel channel = "monotest";

void sender () {
  
  try {
  
    std::cout << "Sender starting..." << std::endl;
    
    LocalClientConnectionRef conn = net.open (sender_id, channel);
    
    boost::mt19937 gen (std::time(0));
    boost::uniform_int<> dist (350, 750);
    boost::variate_generator<boost::mt19937 &, boost::uniform_int<> > rnd (gen, dist);
    
    while (true) {
        const int pause = rnd();
        
        boost::this_thread::sleep
          (boost::posix_time::milliseconds
            (pause));
          
        conn->send (receiver_id, channel, IntMessage (pause));
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
        
        LocalClientConnectionRef conn = net.open (receiver_id, channel);
        
        while (true) {            
            const ParcelRef parcel = conn->receive ();
            std::cout << "Sender " << 
                parcel->envelope ().sender ()
                << " slept " << 
                static_cast<const IntMessage &> (parcel->message ()).get ()
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
  
    std::cout << "vNet started, accepting connections..." << std::endl;
    
    boost::thread sender1   (sender);
    boost::thread receiver1 (receiver);
    
    sender1.join();
    receiver1.join();
    
    return 0;
}