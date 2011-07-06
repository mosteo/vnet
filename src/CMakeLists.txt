set(CMAKE_CXX_FLAGS "-g -Wall")

set(CSD ${CMAKE_CURRENT_SOURCE_DIR}) # Just a shortcut

set(COMMON_SOURCES
    ${CSD}/vnet_client.cpp
    ${CSD}/vnet_core.cpp    
    ${CSD}/vnet_stage.cpp
    ${CSD}/transports/vnet_localtransport.cpp
)

add_executable(vhub ${COMMON_SOURCES} main.cpp)
target_link_libraries(monolithic ${Boost_LIBRARIES})

add_subdirectory(transports)
add_subdirectory(filters)
add_subdirectory(examples)