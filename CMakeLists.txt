cmake_minimum_required(VERSION 2.6)
project(SRE)
if(MSVC)
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO

             CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
             CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
             )
        string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
        string(REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endforeach()

    set(EXTRA_CXX_FLAGS "/std:c++latest")
    set(EXTRA_C_FLAGS "/WX /MP")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-DGTEST_LANG_CXX11=1)
else()
    set(EXTRA_CXX_FLAGS "-std=c++11")
    set(EXTRA_C_FLAGS "-Wall -Werror")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_C_FLAGS} ${EXTRA_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")

if(SRE_DBG)
    set(SRE_COMMAND_LINE_DEFS "${SRE_COMMAND_LINE_DEFS} -DSRE_DBG=1")
else()
    set(SRE_COMMAND_LINE_DEFS "${SRE_COMMAND_LINE_DEFS} -DSRE_DBG=0")
endif()
add_definitions("${SRE_COMMAND_LINE_DEFS}")

add_subdirectory(Modules)
add_subdirectory(Imported)