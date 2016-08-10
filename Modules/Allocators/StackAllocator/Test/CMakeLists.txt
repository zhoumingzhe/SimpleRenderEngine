#include_directories(../../../../Imported/googletest/googletest)
add_executable(TestStackAllocator TestStackAllocator.cpp)
target_link_libraries(TestStackAllocator gtest_main StackAllocator)