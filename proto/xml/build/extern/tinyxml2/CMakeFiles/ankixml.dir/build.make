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
CMAKE_SOURCE_DIR = /home/godlike/src/google-code-repo/proto/xml

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/godlike/src/google-code-repo/proto/xml/build

# Include any dependencies generated for this target.
include extern/tinyxml2/CMakeFiles/ankixml.dir/depend.make

# Include the progress variables for this target.
include extern/tinyxml2/CMakeFiles/ankixml.dir/progress.make

# Include the compile flags for this target's objects.
include extern/tinyxml2/CMakeFiles/ankixml.dir/flags.make

extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o: extern/tinyxml2/CMakeFiles/ankixml.dir/flags.make
extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o: ../extern/tinyxml2/tinyxml2.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/godlike/src/google-code-repo/proto/xml/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o"
	cd /home/godlike/src/google-code-repo/proto/xml/build/extern/tinyxml2 && /usr/local/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ankixml.dir/tinyxml2.cpp.o -c /home/godlike/src/google-code-repo/proto/xml/extern/tinyxml2/tinyxml2.cpp

extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ankixml.dir/tinyxml2.cpp.i"
	cd /home/godlike/src/google-code-repo/proto/xml/build/extern/tinyxml2 && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/godlike/src/google-code-repo/proto/xml/extern/tinyxml2/tinyxml2.cpp > CMakeFiles/ankixml.dir/tinyxml2.cpp.i

extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ankixml.dir/tinyxml2.cpp.s"
	cd /home/godlike/src/google-code-repo/proto/xml/build/extern/tinyxml2 && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/godlike/src/google-code-repo/proto/xml/extern/tinyxml2/tinyxml2.cpp -o CMakeFiles/ankixml.dir/tinyxml2.cpp.s

extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o.requires:
.PHONY : extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o.requires

extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o.provides: extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o.requires
	$(MAKE) -f extern/tinyxml2/CMakeFiles/ankixml.dir/build.make extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o.provides.build
.PHONY : extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o.provides

extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o.provides.build: extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o

# Object files for target ankixml
ankixml_OBJECTS = \
"CMakeFiles/ankixml.dir/tinyxml2.cpp.o"

# External object files for target ankixml
ankixml_EXTERNAL_OBJECTS =

extern/tinyxml2/libankixml.a: extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o
extern/tinyxml2/libankixml.a: extern/tinyxml2/CMakeFiles/ankixml.dir/build.make
extern/tinyxml2/libankixml.a: extern/tinyxml2/CMakeFiles/ankixml.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libankixml.a"
	cd /home/godlike/src/google-code-repo/proto/xml/build/extern/tinyxml2 && $(CMAKE_COMMAND) -P CMakeFiles/ankixml.dir/cmake_clean_target.cmake
	cd /home/godlike/src/google-code-repo/proto/xml/build/extern/tinyxml2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ankixml.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
extern/tinyxml2/CMakeFiles/ankixml.dir/build: extern/tinyxml2/libankixml.a
.PHONY : extern/tinyxml2/CMakeFiles/ankixml.dir/build

extern/tinyxml2/CMakeFiles/ankixml.dir/requires: extern/tinyxml2/CMakeFiles/ankixml.dir/tinyxml2.cpp.o.requires
.PHONY : extern/tinyxml2/CMakeFiles/ankixml.dir/requires

extern/tinyxml2/CMakeFiles/ankixml.dir/clean:
	cd /home/godlike/src/google-code-repo/proto/xml/build/extern/tinyxml2 && $(CMAKE_COMMAND) -P CMakeFiles/ankixml.dir/cmake_clean.cmake
.PHONY : extern/tinyxml2/CMakeFiles/ankixml.dir/clean

extern/tinyxml2/CMakeFiles/ankixml.dir/depend:
	cd /home/godlike/src/google-code-repo/proto/xml/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/godlike/src/google-code-repo/proto/xml /home/godlike/src/google-code-repo/proto/xml/extern/tinyxml2 /home/godlike/src/google-code-repo/proto/xml/build /home/godlike/src/google-code-repo/proto/xml/build/extern/tinyxml2 /home/godlike/src/google-code-repo/proto/xml/build/extern/tinyxml2/CMakeFiles/ankixml.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : extern/tinyxml2/CMakeFiles/ankixml.dir/depend

