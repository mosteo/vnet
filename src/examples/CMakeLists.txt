add_executable(broadcast  ${COMMON_SOURCES} broadcast.cpp)
add_executable(monolithic ${COMMON_SOURCES} monolithic.cpp) 

target_link_libraries(broadcast  ${COMMON_LIBRARIES})
target_link_libraries(monolithic ${COMMON_LIBRARIES})