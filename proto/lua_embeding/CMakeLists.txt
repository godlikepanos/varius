CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(LUA_TEST_PROJECT)

ADD_SUBDIRECTORY(extern)

INCLUDE_DIRECTORIES(extern/lua extern/luabind ${CMAKE_CURRENT_SOURCE_DIR})

SET(CXX_FLAGS "-std=c++0x")

ADD_EXECUTABLE(test-lua Main.cpp Foo.cpp LuaBinder.cpp)
TARGET_LINK_LIBRARIES(test-lua ankilua ankiluabind)
SET_TARGET_PROPERTIES(test-lua PROPERTIES COMPILE_FLAGS ${CXX_FLAGS})
