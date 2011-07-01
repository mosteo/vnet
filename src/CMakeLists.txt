set(CMAKE_CXX_FLAGS "-g -Wall")

add_executable(vhub 
  main.cpp)

add_subdirectory(transports)
add_subdirectory(filters)