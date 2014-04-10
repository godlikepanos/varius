cmake_minimum_required(VERSION 2.8)

include_directories(SDL2/include)
include_directories(GLEW/include)
include_directories(z)
include_directories(include)

add_subdirectory(SDL2)
add_subdirectory(GLEW)
add_subdirectory(z)

add_definitions(-std=c++11 -Wall -Wextra)

add_subdirectory(src/util)
#add_subdirectory(src/gl)

add_executable(asyncgl main.cpp)
target_link_libraries(asyncgl ankisdl ankiglew ankiutil pthread GL)
