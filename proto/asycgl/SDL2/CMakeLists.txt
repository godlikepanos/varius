# General source files 
file(GLOB SDL_COMMON_SRC 
	# Required. It's the same as in SDL CMake
	src/*.c
	src/atomic/*.c
	src/audio/*.c
	src/cpuinfo/*.c
	src/events/*.c
	src/file/*.c
	src/libm/*.c
	src/render/*.c
	src/render/*/*.c
	src/stdlib/*.c
	src/thread/*.c
	src/timer/*.c
	src/video/*.c
	# Extra subsystems
	src/joystick/*.c
	src/haptic/*.c
	src/power/*.c
	src/loadso/*.c)

set(LINUX 1)

if(LINUX)
	add_definitions(-fvisibility=hidden)

	file(GLOB SDL_PLATFORM_SRC 
		src/loadso/dlopen/*.c
		src/video/dummy/*.c
		src/video/x11/*.c
		src/haptic/linux/*.c
		src/joystick/linux/*.c
		src/thread/pthread/*.c
		src/power/linux/*.c
		src/timer/unix/*.c
		src/filesystem/dummy/*.c)
elseif(WINDOWS)
	message(FATAL_ERROR "TODO")
elseif(MACOS)
	message(FATAL_ERROR "TODO")
else()
	message(FATAL_ERROR "Shouldn't reach that point")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(ankisdl ${SDL_COMMON_SRC} ${SDL_PLATFORM_SRC})
target_link_libraries(ankisdl m dl)
