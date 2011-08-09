#include "vnet_stage.h"
#include "vnet_transports.h"

using namespace vnet;

void transport::register_all()
{
    StageFactory::register_stage<LocalTransport>(LocalTransport::name_);
}
