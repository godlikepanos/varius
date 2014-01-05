cmake_minimum_required(VERSION 2.8)

add_subdirectory(SDL2)
add_subdirectory(GLEW)

add_definitions(-std=c++11 -Wall -Wextra)

include_directories(SDL2/include)
include_directories(GLEW/include)

add_executable(asyncgl main.cpp)
target_link_libraries(asyncgl ankisdl ankiglew pthread GL)
