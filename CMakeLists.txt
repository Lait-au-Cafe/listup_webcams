cmake_minimum_required(VERSION 3.0)
project(listup_webcams)
add_executable(listup_webcams source.cpp)

if(__linux__)
    message(STATUS "Platform: Linux")
    SET(CMAKE_CXX_FLAGS "-std=c++11 -g -O3")
endif()

if(MSVC)
    message(STATUS "Platform: Windows")
    target_link_libraries(listup_webcams mf mfplat)
endif()