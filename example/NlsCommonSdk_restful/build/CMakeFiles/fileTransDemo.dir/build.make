# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
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

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful/build

# Include any dependencies generated for this target.
include CMakeFiles/fileTransDemo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fileTransDemo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fileTransDemo.dir/flags.make

CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o: CMakeFiles/fileTransDemo.dir/flags.make
CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o: ../demo/fileTransDemo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o -c /home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful/demo/fileTransDemo.cpp

CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful/demo/fileTransDemo.cpp > CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.i

CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful/demo/fileTransDemo.cpp -o CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.s

CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o.requires:

.PHONY : CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o.requires

CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o.provides: CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o.requires
	$(MAKE) -f CMakeFiles/fileTransDemo.dir/build.make CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o.provides.build
.PHONY : CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o.provides

CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o.provides.build: CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o


# Object files for target fileTransDemo
fileTransDemo_OBJECTS = \
"CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o"

# External object files for target fileTransDemo
fileTransDemo_EXTERNAL_OBJECTS =

../demo/fileTransDemo: CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o
../demo/fileTransDemo: CMakeFiles/fileTransDemo.dir/build.make
../demo/fileTransDemo: CMakeFiles/fileTransDemo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../demo/fileTransDemo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fileTransDemo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fileTransDemo.dir/build: ../demo/fileTransDemo

.PHONY : CMakeFiles/fileTransDemo.dir/build

CMakeFiles/fileTransDemo.dir/requires: CMakeFiles/fileTransDemo.dir/demo/fileTransDemo.cpp.o.requires

.PHONY : CMakeFiles/fileTransDemo.dir/requires

CMakeFiles/fileTransDemo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fileTransDemo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fileTransDemo.dir/clean

CMakeFiles/fileTransDemo.dir/depend:
	cd /home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful /home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful /home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful/build /home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful/build /home/lee/Desktop/musicBox/abnormal-sound-detection/example/NlsCommonSdk_restful/build/CMakeFiles/fileTransDemo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fileTransDemo.dir/depend
