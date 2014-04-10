cmake_minimum_required(VERSION 2.8)

add_definitions(-Wall -Wextra)

add_subdirectory(lua)

add_definitions(-std=c++11)
include_directories("lua")

add_executable(test main.cpp)
target_link_libraries(test ankilua pthread)
