CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_DEFINITIONS(-std=c++0x -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual)

ADD_EXECUTABLE(test Main.cpp)
TARGET_LINK_LIBRARIES(test pthread)
