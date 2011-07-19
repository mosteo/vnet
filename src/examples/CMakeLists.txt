add_executable(broadcast    broadcast.cpp)
add_executable(filteronline filteronline.cpp)
add_executable(monolithic   monolithic.cpp) 

target_link_libraries(broadcast    ${COMMON_LIBRARIES})
target_link_libraries(filteronline ${COMMON_LIBRARIES})
target_link_libraries(monolithic   ${COMMON_LIBRARIES})