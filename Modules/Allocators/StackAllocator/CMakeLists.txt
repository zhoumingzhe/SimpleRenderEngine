include_directories(
..
)
add_library(StackAllocator StackAllocator.h StackAllocator.cpp)
target_link_libraries(StackAllocator OSLayer)
add_subdirectory(Test)