#include "vnet_message.h"

using namespace vnet;

Message::Message (const void *data, const std::size_t size)
{
    data_.resize (size);
    memcpy(&data_[0], data, size);
}