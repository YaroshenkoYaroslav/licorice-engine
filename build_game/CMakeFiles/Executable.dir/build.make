# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/buwnertys/programming/licorice-engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/buwnertys/programming/licorice-engine/build_game

# Include any dependencies generated for this target.
include CMakeFiles/Executable.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Executable.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Executable.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Executable.dir/flags.make

CMakeFiles/Executable.dir/example/main.cpp.o: CMakeFiles/Executable.dir/flags.make
CMakeFiles/Executable.dir/example/main.cpp.o: ../example/main.cpp
CMakeFiles/Executable.dir/example/main.cpp.o: CMakeFiles/Executable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/buwnertys/programming/licorice-engine/build_game/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Executable.dir/example/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Executable.dir/example/main.cpp.o -MF CMakeFiles/Executable.dir/example/main.cpp.o.d -o CMakeFiles/Executable.dir/example/main.cpp.o -c /home/buwnertys/programming/licorice-engine/example/main.cpp

CMakeFiles/Executable.dir/example/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Executable.dir/example/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/buwnertys/programming/licorice-engine/example/main.cpp > CMakeFiles/Executable.dir/example/main.cpp.i

CMakeFiles/Executable.dir/example/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Executable.dir/example/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/buwnertys/programming/licorice-engine/example/main.cpp -o CMakeFiles/Executable.dir/example/main.cpp.s

CMakeFiles/Executable.dir/example/Game.cpp.o: CMakeFiles/Executable.dir/flags.make
CMakeFiles/Executable.dir/example/Game.cpp.o: ../example/Game.cpp
CMakeFiles/Executable.dir/example/Game.cpp.o: CMakeFiles/Executable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/buwnertys/programming/licorice-engine/build_game/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Executable.dir/example/Game.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Executable.dir/example/Game.cpp.o -MF CMakeFiles/Executable.dir/example/Game.cpp.o.d -o CMakeFiles/Executable.dir/example/Game.cpp.o -c /home/buwnertys/programming/licorice-engine/example/Game.cpp

CMakeFiles/Executable.dir/example/Game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Executable.dir/example/Game.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/buwnertys/programming/licorice-engine/example/Game.cpp > CMakeFiles/Executable.dir/example/Game.cpp.i

CMakeFiles/Executable.dir/example/Game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Executable.dir/example/Game.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/buwnertys/programming/licorice-engine/example/Game.cpp -o CMakeFiles/Executable.dir/example/Game.cpp.s

CMakeFiles/Executable.dir/example/Player.cpp.o: CMakeFiles/Executable.dir/flags.make
CMakeFiles/Executable.dir/example/Player.cpp.o: ../example/Player.cpp
CMakeFiles/Executable.dir/example/Player.cpp.o: CMakeFiles/Executable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/buwnertys/programming/licorice-engine/build_game/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Executable.dir/example/Player.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Executable.dir/example/Player.cpp.o -MF CMakeFiles/Executable.dir/example/Player.cpp.o.d -o CMakeFiles/Executable.dir/example/Player.cpp.o -c /home/buwnertys/programming/licorice-engine/example/Player.cpp

CMakeFiles/Executable.dir/example/Player.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Executable.dir/example/Player.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/buwnertys/programming/licorice-engine/example/Player.cpp > CMakeFiles/Executable.dir/example/Player.cpp.i

CMakeFiles/Executable.dir/example/Player.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Executable.dir/example/Player.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/buwnertys/programming/licorice-engine/example/Player.cpp -o CMakeFiles/Executable.dir/example/Player.cpp.s

# Object files for target Executable
Executable_OBJECTS = \
"CMakeFiles/Executable.dir/example/main.cpp.o" \
"CMakeFiles/Executable.dir/example/Game.cpp.o" \
"CMakeFiles/Executable.dir/example/Player.cpp.o"

# External object files for target Executable
Executable_EXTERNAL_OBJECTS =

Executable: CMakeFiles/Executable.dir/example/main.cpp.o
Executable: CMakeFiles/Executable.dir/example/Game.cpp.o
Executable: CMakeFiles/Executable.dir/example/Player.cpp.o
Executable: CMakeFiles/Executable.dir/build.make
Executable: libEngine.a
Executable: libExtlibs.a
Executable: CMakeFiles/Executable.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/buwnertys/programming/licorice-engine/build_game/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable Executable"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Executable.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Executable.dir/build: Executable
.PHONY : CMakeFiles/Executable.dir/build

CMakeFiles/Executable.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Executable.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Executable.dir/clean

CMakeFiles/Executable.dir/depend:
	cd /home/buwnertys/programming/licorice-engine/build_game && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/buwnertys/programming/licorice-engine /home/buwnertys/programming/licorice-engine /home/buwnertys/programming/licorice-engine/build_game /home/buwnertys/programming/licorice-engine/build_game /home/buwnertys/programming/licorice-engine/build_game/CMakeFiles/Executable.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Executable.dir/depend

