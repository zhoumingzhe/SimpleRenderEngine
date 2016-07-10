include_directories(../inc)
add_executable(TestVirtualMemory TestVirtualMemory.cpp)
target_link_libraries(TestVirtualMemory OSLayer)