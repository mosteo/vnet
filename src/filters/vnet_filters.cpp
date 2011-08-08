#include "vnet_stage.h"
#include "vnet_randomfilter.h"
#include "vnet_filters.h"

using namespace vnet;

void filter::register_all()
{
    StageFactory::register_stage<RandomFilter>(RandomFilter::name_);
}
