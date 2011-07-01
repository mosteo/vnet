#include <iostream>

#include "vnet.h"
#include "transports/vnet_localtransport.h"

using namespace vnet;

int main(int argc, char **argv) {
  
    LocalTransport local_transport;
    Network vnet (local_transport);
  
    std::cout << "vNet started, accepting connections." << std::endl;
    return 0;
}