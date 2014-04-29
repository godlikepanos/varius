cmake_minimum_required(VERSION 2.8)

include_directories(GLEW/include)

add_subdirectory(GLEW)

add_definitions(-std=c99)

add_executable(testgl main.c)
target_link_libraries(testgl SDL2 ankiglew pthread GL)
