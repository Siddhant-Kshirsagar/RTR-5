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
include CMakeFiles/joystick_static.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/joystick_static.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/joystick_static.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/joystick_static.dir/flags.make

CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.o: CMakeFiles/joystick_static.dir/flags.make
CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.o: progs/demos/joystick/joystick.c
CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.o: CMakeFiles/joystick_static.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.o -MF CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.o.d -o CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.o -c /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/progs/demos/joystick/joystick.c

CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/progs/demos/joystick/joystick.c > CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.i

CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/progs/demos/joystick/joystick.c -o CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.s

# Object files for target joystick_static
joystick_static_OBJECTS = \
"CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.o"

# External object files for target joystick_static
joystick_static_EXTERNAL_OBJECTS =

bin/joystick_static: CMakeFiles/joystick_static.dir/progs/demos/joystick/joystick.c.o
bin/joystick_static: CMakeFiles/joystick_static.dir/build.make
bin/joystick_static: /usr/lib/x86_64-linux-gnu/libGLU.so
bin/joystick_static: /usr/lib/x86_64-linux-gnu/libX11.so
bin/joystick_static: /usr/lib/x86_64-linux-gnu/libXrandr.so
bin/joystick_static: /usr/lib/x86_64-linux-gnu/libXi.so
bin/joystick_static: /usr/lib/x86_64-linux-gnu/libGL.so
bin/joystick_static: lib/libglut.a
bin/joystick_static: /usr/lib/x86_64-linux-gnu/libX11.so
bin/joystick_static: /usr/lib/x86_64-linux-gnu/libXrandr.so
bin/joystick_static: /usr/lib/x86_64-linux-gnu/libXi.so
bin/joystick_static: /usr/lib/x86_64-linux-gnu/libGL.so
bin/joystick_static: CMakeFiles/joystick_static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin/joystick_static"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/joystick_static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/joystick_static.dir/build: bin/joystick_static
.PHONY : CMakeFiles/joystick_static.dir/build

CMakeFiles/joystick_static.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/joystick_static.dir/cmake_clean.cmake
.PHONY : CMakeFiles/joystick_static.dir/clean

CMakeFiles/joystick_static.dir/depend:
	cd /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0 /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0 /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0 /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0 /home/hemant/Siddhant/RTR/01-XWindows/03-FreeGlut/freeglut-3.4.0/CMakeFiles/joystick_static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/joystick_static.dir/depend

