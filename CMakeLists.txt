cmake_minimum_required(VERSION 2.6)
project(pbre)
if(MSVC)
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
        string(REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endforeach()

    set(EXTRA_CXX_FLAGS)
    set(EXTRA_C_FLAGS "/WX /MP")
else()
    set(EXTRA_CXX_FLAGS -std=c++11)
    set(EXTRA_C_FLAGS -Wall -Werror)
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_C_FLAGS} ${EXTRA_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")

add_subdirectory(Modules)
add_subdirectory(Imported)