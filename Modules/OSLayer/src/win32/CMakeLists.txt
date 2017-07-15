add_library(OSLayer BackTrace.cpp VirtualMemory.cpp DebugInformation.cpp)

target_include_directories(OSLayer PUBLIC ../../inc)