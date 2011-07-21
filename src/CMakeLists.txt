set(CMAKE_CXX_FLAGS "-g -Wall -O2")

find_package(Boost COMPONENTS thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(vnet_common
            vnet_client.cpp
            vnet_core.cpp  
            vnet_message.cpp
            vnet_stage.cpp
)

set(COMMON_LIBRARIES
    vnet_common
    vnet_filters
    vnet_transports
    ${Boost_LIBRARIES}
)

add_subdirectory(filters)
add_subdirectory(transports)
add_subdirectory(examples)