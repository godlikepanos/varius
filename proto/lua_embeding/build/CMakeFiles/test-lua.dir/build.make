# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/godlike/src/google-code-repo/proto/lua-embedding

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/godlike/src/google-code-repo/proto/lua-embedding/build

# Include any dependencies generated for this target.
include CMakeFiles/test-lua.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test-lua.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test-lua.dir/flags.make

CMakeFiles/test-lua.dir/Main.cpp.o: CMakeFiles/test-lua.dir/flags.make
CMakeFiles/test-lua.dir/Main.cpp.o: ../Main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/godlike/src/google-code-repo/proto/lua-embedding/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/test-lua.dir/Main.cpp.o"
	/opt/llvm-3.1/bin/clang++   $(CXX_DEFINES) $(CXX_FLAGS) -std=c++0x -o CMakeFiles/test-lua.dir/Main.cpp.o -c /home/godlike/src/google-code-repo/proto/lua-embedding/Main.cpp

CMakeFiles/test-lua.dir/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-lua.dir/Main.cpp.i"
	/opt/llvm-3.1/bin/clang++  $(CXX_DEFINES) $(CXX_FLAGS) -std=c++0x -E /home/godlike/src/google-code-repo/proto/lua-embedding/Main.cpp > CMakeFiles/test-lua.dir/Main.cpp.i

CMakeFiles/test-lua.dir/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-lua.dir/Main.cpp.s"
	/opt/llvm-3.1/bin/clang++  $(CXX_DEFINES) $(CXX_FLAGS) -std=c++0x -S /home/godlike/src/google-code-repo/proto/lua-embedding/Main.cpp -o CMakeFiles/test-lua.dir/Main.cpp.s

CMakeFiles/test-lua.dir/Main.cpp.o.requires:
.PHONY : CMakeFiles/test-lua.dir/Main.cpp.o.requires

CMakeFiles/test-lua.dir/Main.cpp.o.provides: CMakeFiles/test-lua.dir/Main.cpp.o.requires
	$(MAKE) -f CMakeFiles/test-lua.dir/build.make CMakeFiles/test-lua.dir/Main.cpp.o.provides.build
.PHONY : CMakeFiles/test-lua.dir/Main.cpp.o.provides

CMakeFiles/test-lua.dir/Main.cpp.o.provides.build: CMakeFiles/test-lua.dir/Main.cpp.o

CMakeFiles/test-lua.dir/Foo.cpp.o: CMakeFiles/test-lua.dir/flags.make
CMakeFiles/test-lua.dir/Foo.cpp.o: ../Foo.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/godlike/src/google-code-repo/proto/lua-embedding/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/test-lua.dir/Foo.cpp.o"
	/opt/llvm-3.1/bin/clang++   $(CXX_DEFINES) $(CXX_FLAGS) -std=c++0x -o CMakeFiles/test-lua.dir/Foo.cpp.o -c /home/godlike/src/google-code-repo/proto/lua-embedding/Foo.cpp

CMakeFiles/test-lua.dir/Foo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-lua.dir/Foo.cpp.i"
	/opt/llvm-3.1/bin/clang++  $(CXX_DEFINES) $(CXX_FLAGS) -std=c++0x -E /home/godlike/src/google-code-repo/proto/lua-embedding/Foo.cpp > CMakeFiles/test-lua.dir/Foo.cpp.i

CMakeFiles/test-lua.dir/Foo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-lua.dir/Foo.cpp.s"
	/opt/llvm-3.1/bin/clang++  $(CXX_DEFINES) $(CXX_FLAGS) -std=c++0x -S /home/godlike/src/google-code-repo/proto/lua-embedding/Foo.cpp -o CMakeFiles/test-lua.dir/Foo.cpp.s

CMakeFiles/test-lua.dir/Foo.cpp.o.requires:
.PHONY : CMakeFiles/test-lua.dir/Foo.cpp.o.requires

CMakeFiles/test-lua.dir/Foo.cpp.o.provides: CMakeFiles/test-lua.dir/Foo.cpp.o.requires
	$(MAKE) -f CMakeFiles/test-lua.dir/build.make CMakeFiles/test-lua.dir/Foo.cpp.o.provides.build
.PHONY : CMakeFiles/test-lua.dir/Foo.cpp.o.provides

CMakeFiles/test-lua.dir/Foo.cpp.o.provides.build: CMakeFiles/test-lua.dir/Foo.cpp.o

# Object files for target test-lua
test__lua_OBJECTS = \
"CMakeFiles/test-lua.dir/Main.cpp.o" \
"CMakeFiles/test-lua.dir/Foo.cpp.o"

# External object files for target test-lua
test__lua_EXTERNAL_OBJECTS =

test-lua: CMakeFiles/test-lua.dir/Main.cpp.o
test-lua: CMakeFiles/test-lua.dir/Foo.cpp.o
test-lua: extern/lua/libankilua.a
test-lua: extern/luabind/libankiluabind.a
test-lua: CMakeFiles/test-lua.dir/build.make
test-lua: CMakeFiles/test-lua.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable test-lua"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-lua.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test-lua.dir/build: test-lua
.PHONY : CMakeFiles/test-lua.dir/build

CMakeFiles/test-lua.dir/requires: CMakeFiles/test-lua.dir/Main.cpp.o.requires
CMakeFiles/test-lua.dir/requires: CMakeFiles/test-lua.dir/Foo.cpp.o.requires
.PHONY : CMakeFiles/test-lua.dir/requires

CMakeFiles/test-lua.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test-lua.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test-lua.dir/clean

CMakeFiles/test-lua.dir/depend:
	cd /home/godlike/src/google-code-repo/proto/lua-embedding/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/godlike/src/google-code-repo/proto/lua-embedding /home/godlike/src/google-code-repo/proto/lua-embedding /home/godlike/src/google-code-repo/proto/lua-embedding/build /home/godlike/src/google-code-repo/proto/lua-embedding/build /home/godlike/src/google-code-repo/proto/lua-embedding/build/CMakeFiles/test-lua.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test-lua.dir/depend

