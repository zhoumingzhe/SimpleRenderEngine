add_library(
OSLayer
BackTrace.cpp
DebugInformationLibrary.cpp
VirtualMemory.cpp
)

target_include_directories(OSLayer PUBLIC ../../inc)
target_link_libraries(OSLayer dl bfd)
