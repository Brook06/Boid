add_executable(main main.cpp)
add_executable(main1 main1.cpp)

target_link_libraries(main PRIVATE Robot)
target_link_libraries(main1 PRIVATE Robot)