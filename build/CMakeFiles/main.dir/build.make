# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_SOURCE_DIR = /home/mauro/Documents/computacion-grafica/tp-final

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mauro/Documents/computacion-grafica/tp-final/build

# Include any dependencies generated for this target.
include CMakeFiles/main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.dir/flags.make

CMakeFiles/main.dir/src/hittables/space_divider.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/hittables/space_divider.cpp.o: ../src/hittables/space_divider.cpp
CMakeFiles/main.dir/src/hittables/space_divider.cpp.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mauro/Documents/computacion-grafica/tp-final/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main.dir/src/hittables/space_divider.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/src/hittables/space_divider.cpp.o -MF CMakeFiles/main.dir/src/hittables/space_divider.cpp.o.d -o CMakeFiles/main.dir/src/hittables/space_divider.cpp.o -c /home/mauro/Documents/computacion-grafica/tp-final/src/hittables/space_divider.cpp

CMakeFiles/main.dir/src/hittables/space_divider.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/hittables/space_divider.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mauro/Documents/computacion-grafica/tp-final/src/hittables/space_divider.cpp > CMakeFiles/main.dir/src/hittables/space_divider.cpp.i

CMakeFiles/main.dir/src/hittables/space_divider.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/hittables/space_divider.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mauro/Documents/computacion-grafica/tp-final/src/hittables/space_divider.cpp -o CMakeFiles/main.dir/src/hittables/space_divider.cpp.s

CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.o: ../src/hittables/triangle_mesh.cpp
CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mauro/Documents/computacion-grafica/tp-final/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.o -MF CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.o.d -o CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.o -c /home/mauro/Documents/computacion-grafica/tp-final/src/hittables/triangle_mesh.cpp

CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mauro/Documents/computacion-grafica/tp-final/src/hittables/triangle_mesh.cpp > CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.i

CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mauro/Documents/computacion-grafica/tp-final/src/hittables/triangle_mesh.cpp -o CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.s

CMakeFiles/main.dir/main.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/main.cpp.o: ../main.cpp
CMakeFiles/main.dir/main.cpp.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mauro/Documents/computacion-grafica/tp-final/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/main.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/main.cpp.o -MF CMakeFiles/main.dir/main.cpp.o.d -o CMakeFiles/main.dir/main.cpp.o -c /home/mauro/Documents/computacion-grafica/tp-final/main.cpp

CMakeFiles/main.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mauro/Documents/computacion-grafica/tp-final/main.cpp > CMakeFiles/main.dir/main.cpp.i

CMakeFiles/main.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mauro/Documents/computacion-grafica/tp-final/main.cpp -o CMakeFiles/main.dir/main.cpp.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/src/hittables/space_divider.cpp.o" \
"CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.o" \
"CMakeFiles/main.dir/main.cpp.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

main: CMakeFiles/main.dir/src/hittables/space_divider.cpp.o
main: CMakeFiles/main.dir/src/hittables/triangle_mesh.cpp.o
main: CMakeFiles/main.dir/main.cpp.o
main: CMakeFiles/main.dir/build.make
main: CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mauro/Documents/computacion-grafica/tp-final/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.dir/build: main
.PHONY : CMakeFiles/main.dir/build

CMakeFiles/main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.dir/clean

CMakeFiles/main.dir/depend:
	cd /home/mauro/Documents/computacion-grafica/tp-final/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mauro/Documents/computacion-grafica/tp-final /home/mauro/Documents/computacion-grafica/tp-final /home/mauro/Documents/computacion-grafica/tp-final/build /home/mauro/Documents/computacion-grafica/tp-final/build /home/mauro/Documents/computacion-grafica/tp-final/build/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main.dir/depend

