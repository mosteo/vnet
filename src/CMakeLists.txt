set(CMAKE_CXX_FLAGS "-g -Wall -O2")

find_package(Boost COMPONENTS thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

set(CSD ${CMAKE_CURRENT_SOURCE_DIR}) # Just a shortcut

set(COMMON_SOURCES
    ${CSD}/vnet_client.cpp
    ${CSD}/vnet_core.cpp    
    ${CSD}/vnet_stage.cpp
    ${CSD}/transports/vnet_localtransport.cpp
)

set(COMMON_LIBRARIES ${Boost_LIBRARIES})

#add_executable(vhub ${COMMON_SOURCES} main.cpp)

add_subdirectory(transports)
add_subdirectory(filters)
add_subdirectory(examples)