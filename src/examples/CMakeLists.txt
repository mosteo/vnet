find_package(Boost COMPONENTS thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(monolithic ${COMMON_SOURCES} monolithic.cpp)

target_link_libraries(monolithic ${Boost_LIBRARIES})
