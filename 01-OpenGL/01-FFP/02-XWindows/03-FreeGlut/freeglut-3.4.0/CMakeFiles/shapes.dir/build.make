# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0

# Include any dependencies generated for this target.
include CMakeFiles/shapes.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/shapes.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/shapes.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/shapes.dir/flags.make

CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.o: CMakeFiles/shapes.dir/flags.make
CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.o: progs/demos/shapes/shapes.c
CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.o: CMakeFiles/shapes.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.o -MF CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.o.d -o CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.o -c /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/progs/demos/shapes/shapes.c

CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/progs/demos/shapes/shapes.c > CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.i

CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/progs/demos/shapes/shapes.c -o CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.s

CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.o: CMakeFiles/shapes.dir/flags.make
CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.o: progs/demos/shapes/glmatrix.c
CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.o: CMakeFiles/shapes.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.o -MF CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.o.d -o CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.o -c /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/progs/demos/shapes/glmatrix.c

CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/progs/demos/shapes/glmatrix.c > CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.i

CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/progs/demos/shapes/glmatrix.c -o CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.s

# Object files for target shapes
shapes_OBJECTS = \
"CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.o" \
"CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.o"

# External object files for target shapes
shapes_EXTERNAL_OBJECTS =

bin/shapes: CMakeFiles/shapes.dir/progs/demos/shapes/shapes.c.o
bin/shapes: CMakeFiles/shapes.dir/progs/demos/shapes/glmatrix.c.o
bin/shapes: CMakeFiles/shapes.dir/build.make
bin/shapes: /usr/lib/x86_64-linux-gnu/libGLU.so
bin/shapes: /usr/lib/x86_64-linux-gnu/libX11.so
bin/shapes: /usr/lib/x86_64-linux-gnu/libXrandr.so
bin/shapes: /usr/lib/x86_64-linux-gnu/libXi.so
bin/shapes: /usr/lib/x86_64-linux-gnu/libGL.so
bin/shapes: lib/libglut.so.3.12.0
bin/shapes: /usr/lib/x86_64-linux-gnu/libX11.so
bin/shapes: /usr/lib/x86_64-linux-gnu/libXrandr.so
bin/shapes: /usr/lib/x86_64-linux-gnu/libXi.so
bin/shapes: /usr/lib/x86_64-linux-gnu/libGL.so
bin/shapes: CMakeFiles/shapes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable bin/shapes"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/shapes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/shapes.dir/build: bin/shapes
.PHONY : CMakeFiles/shapes.dir/build

CMakeFiles/shapes.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/shapes.dir/cmake_clean.cmake
.PHONY : CMakeFiles/shapes.dir/clean

CMakeFiles/shapes.dir/depend:
	cd /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0 /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0 /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0 /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0 /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/CMakeFiles/shapes.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/shapes.dir/depend

