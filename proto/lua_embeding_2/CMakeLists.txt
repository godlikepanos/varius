cmake_minimum_required(VERSION 2.8)

add_definitions(-pedantic -Wall -W -Wextra -Wwrite-strings -Wunused -Wunused-variable -Wno-unused-parameter -Wundef)

add_subdirectory(lua)

add_definitions(-std=c++11)
#add_definitions(--gcc-toolchain=/scratch/pancha01/opt/gcc-4.8/)
include_directories("lua")

add_executable(test main.cpp)
target_link_libraries(test ankilua pthread)
