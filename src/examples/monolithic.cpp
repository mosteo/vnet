#include <boost/array.hpp>
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

boost::array<NodeId, 2> nodes = { {"Ari", "Ben"} };

const Channel channel = "monotest";

void client(int index) {
  
  try {
  
    const NodeId id (nodes.at (index));
  
    std::cout << "Client " << id << " starting..." << std::endl;
    
    LocalClientConnectionRef conn = net.open (id);
    
    boost::mt19937 gen (std::time(0));
    boost::uniform_int<> dist (350, 750);
    boost::variate_generator<boost::mt19937 &, boost::uniform_int<> > rnd (gen, dist);
    
    while (true) {
	const int pause = rnd();
	
	boost::this_thread::sleep
	  (boost::posix_time::milliseconds
	    (pause));
	  
	conn->send (UnicastDestination (nodes.at (1 - index), channel), IntMessage (pause));
    }
    
  } 
  catch (std::exception &e) {
    std::cout << "Exception [client]: " << e.what() << std::endl;
  }
  catch (...) {
    std::cout << "Unknown exception." << std::endl;
  }
}

int main(int argc, char **argv) {
  
    std::cout << "vNet started, accepting connections..." << std::endl;
    
    boost::thread client1 (client, 0);
    boost::thread client2 (client, 1);
    
    client1.join();
    client2.join();
    
    return 0;
}