# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /opt/ohpc/pub/software/hdevarajan/spack3/v0.15.4.scs/opt/spack/linux-centos7-skylake_avx512/gcc-9.3.0/cmake/3.17.3/bin/cmake

# The command to remove a file.
RM = /opt/ohpc/pub/software/hdevarajan/spack3/v0.15.4.scs/opt/spack/linux-centos7-skylake_avx512/gcc-9.3.0/cmake/3.17.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jcernudagarcia/Apollo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jcernudagarcia/Apollo/build

# Include any dependencies generated for this target.
include CMakeFiles/subscribe_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/subscribe_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/subscribe_test.dir/flags.make

CMakeFiles/subscribe_test.dir/src/queue.cpp.o: CMakeFiles/subscribe_test.dir/flags.make
CMakeFiles/subscribe_test.dir/src/queue.cpp.o: ../src/queue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcernudagarcia/Apollo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/subscribe_test.dir/src/queue.cpp.o"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/subscribe_test.dir/src/queue.cpp.o -c /home/jcernudagarcia/Apollo/src/queue.cpp

CMakeFiles/subscribe_test.dir/src/queue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribe_test.dir/src/queue.cpp.i"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcernudagarcia/Apollo/src/queue.cpp > CMakeFiles/subscribe_test.dir/src/queue.cpp.i

CMakeFiles/subscribe_test.dir/src/queue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribe_test.dir/src/queue.cpp.s"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcernudagarcia/Apollo/src/queue.cpp -o CMakeFiles/subscribe_test.dir/src/queue.cpp.s

CMakeFiles/subscribe_test.dir/src/queue_key.cpp.o: CMakeFiles/subscribe_test.dir/flags.make
CMakeFiles/subscribe_test.dir/src/queue_key.cpp.o: ../src/queue_key.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcernudagarcia/Apollo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/subscribe_test.dir/src/queue_key.cpp.o"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/subscribe_test.dir/src/queue_key.cpp.o -c /home/jcernudagarcia/Apollo/src/queue_key.cpp

CMakeFiles/subscribe_test.dir/src/queue_key.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribe_test.dir/src/queue_key.cpp.i"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcernudagarcia/Apollo/src/queue_key.cpp > CMakeFiles/subscribe_test.dir/src/queue_key.cpp.i

CMakeFiles/subscribe_test.dir/src/queue_key.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribe_test.dir/src/queue_key.cpp.s"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcernudagarcia/Apollo/src/queue_key.cpp -o CMakeFiles/subscribe_test.dir/src/queue_key.cpp.s

CMakeFiles/subscribe_test.dir/src/queue_type.cpp.o: CMakeFiles/subscribe_test.dir/flags.make
CMakeFiles/subscribe_test.dir/src/queue_type.cpp.o: ../src/queue_type.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcernudagarcia/Apollo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/subscribe_test.dir/src/queue_type.cpp.o"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/subscribe_test.dir/src/queue_type.cpp.o -c /home/jcernudagarcia/Apollo/src/queue_type.cpp

CMakeFiles/subscribe_test.dir/src/queue_type.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribe_test.dir/src/queue_type.cpp.i"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcernudagarcia/Apollo/src/queue_type.cpp > CMakeFiles/subscribe_test.dir/src/queue_type.cpp.i

CMakeFiles/subscribe_test.dir/src/queue_type.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribe_test.dir/src/queue_type.cpp.s"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcernudagarcia/Apollo/src/queue_type.cpp -o CMakeFiles/subscribe_test.dir/src/queue_type.cpp.s

CMakeFiles/subscribe_test.dir/src/mon_hooks.cpp.o: CMakeFiles/subscribe_test.dir/flags.make
CMakeFiles/subscribe_test.dir/src/mon_hooks.cpp.o: ../src/mon_hooks.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcernudagarcia/Apollo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/subscribe_test.dir/src/mon_hooks.cpp.o"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/subscribe_test.dir/src/mon_hooks.cpp.o -c /home/jcernudagarcia/Apollo/src/mon_hooks.cpp

CMakeFiles/subscribe_test.dir/src/mon_hooks.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribe_test.dir/src/mon_hooks.cpp.i"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcernudagarcia/Apollo/src/mon_hooks.cpp > CMakeFiles/subscribe_test.dir/src/mon_hooks.cpp.i

CMakeFiles/subscribe_test.dir/src/mon_hooks.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribe_test.dir/src/mon_hooks.cpp.s"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcernudagarcia/Apollo/src/mon_hooks.cpp -o CMakeFiles/subscribe_test.dir/src/mon_hooks.cpp.s

CMakeFiles/subscribe_test.dir/src/redis.cpp.o: CMakeFiles/subscribe_test.dir/flags.make
CMakeFiles/subscribe_test.dir/src/redis.cpp.o: ../src/redis.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcernudagarcia/Apollo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/subscribe_test.dir/src/redis.cpp.o"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/subscribe_test.dir/src/redis.cpp.o -c /home/jcernudagarcia/Apollo/src/redis.cpp

CMakeFiles/subscribe_test.dir/src/redis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribe_test.dir/src/redis.cpp.i"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcernudagarcia/Apollo/src/redis.cpp > CMakeFiles/subscribe_test.dir/src/redis.cpp.i

CMakeFiles/subscribe_test.dir/src/redis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribe_test.dir/src/redis.cpp.s"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcernudagarcia/Apollo/src/redis.cpp -o CMakeFiles/subscribe_test.dir/src/redis.cpp.s

CMakeFiles/subscribe_test.dir/src/queue_config.cpp.o: CMakeFiles/subscribe_test.dir/flags.make
CMakeFiles/subscribe_test.dir/src/queue_config.cpp.o: ../src/queue_config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcernudagarcia/Apollo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/subscribe_test.dir/src/queue_config.cpp.o"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/subscribe_test.dir/src/queue_config.cpp.o -c /home/jcernudagarcia/Apollo/src/queue_config.cpp

CMakeFiles/subscribe_test.dir/src/queue_config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribe_test.dir/src/queue_config.cpp.i"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcernudagarcia/Apollo/src/queue_config.cpp > CMakeFiles/subscribe_test.dir/src/queue_config.cpp.i

CMakeFiles/subscribe_test.dir/src/queue_config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribe_test.dir/src/queue_config.cpp.s"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcernudagarcia/Apollo/src/queue_config.cpp -o CMakeFiles/subscribe_test.dir/src/queue_config.cpp.s

CMakeFiles/subscribe_test.dir/src/s_queues.cpp.o: CMakeFiles/subscribe_test.dir/flags.make
CMakeFiles/subscribe_test.dir/src/s_queues.cpp.o: ../src/s_queues.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcernudagarcia/Apollo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/subscribe_test.dir/src/s_queues.cpp.o"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/subscribe_test.dir/src/s_queues.cpp.o -c /home/jcernudagarcia/Apollo/src/s_queues.cpp

CMakeFiles/subscribe_test.dir/src/s_queues.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribe_test.dir/src/s_queues.cpp.i"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcernudagarcia/Apollo/src/s_queues.cpp > CMakeFiles/subscribe_test.dir/src/s_queues.cpp.i

CMakeFiles/subscribe_test.dir/src/s_queues.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribe_test.dir/src/s_queues.cpp.s"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcernudagarcia/Apollo/src/s_queues.cpp -o CMakeFiles/subscribe_test.dir/src/s_queues.cpp.s

CMakeFiles/subscribe_test.dir/src/Pythio.cpp.o: CMakeFiles/subscribe_test.dir/flags.make
CMakeFiles/subscribe_test.dir/src/Pythio.cpp.o: ../src/Pythio.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcernudagarcia/Apollo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/subscribe_test.dir/src/Pythio.cpp.o"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/subscribe_test.dir/src/Pythio.cpp.o -c /home/jcernudagarcia/Apollo/src/Pythio.cpp

CMakeFiles/subscribe_test.dir/src/Pythio.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribe_test.dir/src/Pythio.cpp.i"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcernudagarcia/Apollo/src/Pythio.cpp > CMakeFiles/subscribe_test.dir/src/Pythio.cpp.i

CMakeFiles/subscribe_test.dir/src/Pythio.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribe_test.dir/src/Pythio.cpp.s"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcernudagarcia/Apollo/src/Pythio.cpp -o CMakeFiles/subscribe_test.dir/src/Pythio.cpp.s

CMakeFiles/subscribe_test.dir/src/timer.cpp.o: CMakeFiles/subscribe_test.dir/flags.make
CMakeFiles/subscribe_test.dir/src/timer.cpp.o: ../src/timer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcernudagarcia/Apollo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/subscribe_test.dir/src/timer.cpp.o"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/subscribe_test.dir/src/timer.cpp.o -c /home/jcernudagarcia/Apollo/src/timer.cpp

CMakeFiles/subscribe_test.dir/src/timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribe_test.dir/src/timer.cpp.i"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcernudagarcia/Apollo/src/timer.cpp > CMakeFiles/subscribe_test.dir/src/timer.cpp.i

CMakeFiles/subscribe_test.dir/src/timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribe_test.dir/src/timer.cpp.s"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcernudagarcia/Apollo/src/timer.cpp -o CMakeFiles/subscribe_test.dir/src/timer.cpp.s

CMakeFiles/subscribe_test.dir/scs_io_common/src/common/debug.cpp.o: CMakeFiles/subscribe_test.dir/flags.make
CMakeFiles/subscribe_test.dir/scs_io_common/src/common/debug.cpp.o: ../scs_io_common/src/common/debug.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcernudagarcia/Apollo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/subscribe_test.dir/scs_io_common/src/common/debug.cpp.o"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/subscribe_test.dir/scs_io_common/src/common/debug.cpp.o -c /home/jcernudagarcia/Apollo/scs_io_common/src/common/debug.cpp

CMakeFiles/subscribe_test.dir/scs_io_common/src/common/debug.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribe_test.dir/scs_io_common/src/common/debug.cpp.i"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcernudagarcia/Apollo/scs_io_common/src/common/debug.cpp > CMakeFiles/subscribe_test.dir/scs_io_common/src/common/debug.cpp.i

CMakeFiles/subscribe_test.dir/scs_io_common/src/common/debug.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribe_test.dir/scs_io_common/src/common/debug.cpp.s"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcernudagarcia/Apollo/scs_io_common/src/common/debug.cpp -o CMakeFiles/subscribe_test.dir/scs_io_common/src/common/debug.cpp.s

CMakeFiles/subscribe_test.dir/tests/subscribe_test.cpp.o: CMakeFiles/subscribe_test.dir/flags.make
CMakeFiles/subscribe_test.dir/tests/subscribe_test.cpp.o: ../tests/subscribe_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcernudagarcia/Apollo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/subscribe_test.dir/tests/subscribe_test.cpp.o"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/subscribe_test.dir/tests/subscribe_test.cpp.o -c /home/jcernudagarcia/Apollo/tests/subscribe_test.cpp

CMakeFiles/subscribe_test.dir/tests/subscribe_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribe_test.dir/tests/subscribe_test.cpp.i"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcernudagarcia/Apollo/tests/subscribe_test.cpp > CMakeFiles/subscribe_test.dir/tests/subscribe_test.cpp.i

CMakeFiles/subscribe_test.dir/tests/subscribe_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribe_test.dir/tests/subscribe_test.cpp.s"
	/opt/ohpc/pub/compiler/gcc/7.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcernudagarcia/Apollo/tests/subscribe_test.cpp -o CMakeFiles/subscribe_test.dir/tests/subscribe_test.cpp.s

# Object files for target subscribe_test
subscribe_test_OBJECTS = \
"CMakeFiles/subscribe_test.dir/src/queue.cpp.o" \
"CMakeFiles/subscribe_test.dir/src/queue_key.cpp.o" \
"CMakeFiles/subscribe_test.dir/src/queue_type.cpp.o" \
"CMakeFiles/subscribe_test.dir/src/mon_hooks.cpp.o" \
"CMakeFiles/subscribe_test.dir/src/redis.cpp.o" \
"CMakeFiles/subscribe_test.dir/src/queue_config.cpp.o" \
"CMakeFiles/subscribe_test.dir/src/s_queues.cpp.o" \
"CMakeFiles/subscribe_test.dir/src/Pythio.cpp.o" \
"CMakeFiles/subscribe_test.dir/src/timer.cpp.o" \
"CMakeFiles/subscribe_test.dir/scs_io_common/src/common/debug.cpp.o" \
"CMakeFiles/subscribe_test.dir/tests/subscribe_test.cpp.o"

# External object files for target subscribe_test
subscribe_test_EXTERNAL_OBJECTS =

subscribe_test: CMakeFiles/subscribe_test.dir/src/queue.cpp.o
subscribe_test: CMakeFiles/subscribe_test.dir/src/queue_key.cpp.o
subscribe_test: CMakeFiles/subscribe_test.dir/src/queue_type.cpp.o
subscribe_test: CMakeFiles/subscribe_test.dir/src/mon_hooks.cpp.o
subscribe_test: CMakeFiles/subscribe_test.dir/src/redis.cpp.o
subscribe_test: CMakeFiles/subscribe_test.dir/src/queue_config.cpp.o
subscribe_test: CMakeFiles/subscribe_test.dir/src/s_queues.cpp.o
subscribe_test: CMakeFiles/subscribe_test.dir/src/Pythio.cpp.o
subscribe_test: CMakeFiles/subscribe_test.dir/src/timer.cpp.o
subscribe_test: CMakeFiles/subscribe_test.dir/scs_io_common/src/common/debug.cpp.o
subscribe_test: CMakeFiles/subscribe_test.dir/tests/subscribe_test.cpp.o
subscribe_test: CMakeFiles/subscribe_test.dir/build.make
subscribe_test: CMakeFiles/subscribe_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jcernudagarcia/Apollo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable subscribe_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/subscribe_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/subscribe_test.dir/build: subscribe_test

.PHONY : CMakeFiles/subscribe_test.dir/build

CMakeFiles/subscribe_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/subscribe_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/subscribe_test.dir/clean

CMakeFiles/subscribe_test.dir/depend:
	cd /home/jcernudagarcia/Apollo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jcernudagarcia/Apollo /home/jcernudagarcia/Apollo /home/jcernudagarcia/Apollo/build /home/jcernudagarcia/Apollo/build /home/jcernudagarcia/Apollo/build/CMakeFiles/subscribe_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/subscribe_test.dir/depend

