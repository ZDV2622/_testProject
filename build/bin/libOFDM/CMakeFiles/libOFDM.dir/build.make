# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build

# Include any dependencies generated for this target.
include bin/libOFDM/CMakeFiles/libOFDM.dir/depend.make

# Include the progress variables for this target.
include bin/libOFDM/CMakeFiles/libOFDM.dir/progress.make

# Include the compile flags for this target's objects.
include bin/libOFDM/CMakeFiles/libOFDM.dir/flags.make

bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp.o: bin/libOFDM/CMakeFiles/libOFDM.dir/flags.make
bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp.o: /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp.o"
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp.o -c /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp

bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp.i"
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp > CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp.i

bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp.s"
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp -o CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp.s

bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp.o: bin/libOFDM/CMakeFiles/libOFDM.dir/flags.make
bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp.o: /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp.o"
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp.o -c /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp

bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp.i"
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp > CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp.i

bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp.s"
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp -o CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp.s

bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp.o: bin/libOFDM/CMakeFiles/libOFDM.dir/flags.make
bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp.o: /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp.o"
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp.o -c /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp

bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp.i"
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp > CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp.i

bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp.s"
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp -o CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp.s

# Object files for target libOFDM
libOFDM_OBJECTS = \
"CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp.o" \
"CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp.o" \
"CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp.o"

# External object files for target libOFDM
libOFDM_EXTERNAL_OBJECTS =

bin/libOFDM/liblibOFDM.a: bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/_fft.cpp.o
bin/libOFDM/liblibOFDM.a: bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/fft_ifft_radixN.cpp.o
bin/libOFDM/liblibOFDM.a: bin/libOFDM/CMakeFiles/libOFDM.dir/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/sources/ofdm_modulation.cpp.o
bin/libOFDM/liblibOFDM.a: bin/libOFDM/CMakeFiles/libOFDM.dir/build.make
bin/libOFDM/liblibOFDM.a: bin/libOFDM/CMakeFiles/libOFDM.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library liblibOFDM.a"
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM && $(CMAKE_COMMAND) -P CMakeFiles/libOFDM.dir/cmake_clean_target.cmake
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libOFDM.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bin/libOFDM/CMakeFiles/libOFDM.dir/build: bin/libOFDM/liblibOFDM.a

.PHONY : bin/libOFDM/CMakeFiles/libOFDM.dir/build

bin/libOFDM/CMakeFiles/libOFDM.dir/clean:
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM && $(CMAKE_COMMAND) -P CMakeFiles/libOFDM.dir/cmake_clean.cmake
.PHONY : bin/libOFDM/CMakeFiles/libOFDM.dir/clean

bin/libOFDM/CMakeFiles/libOFDM.dir/depend:
	cd /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/libOFDM/build /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM /home/darya/C_projects/NR_5G_3GPP_Prod_cmake/build/bin/libOFDM/CMakeFiles/libOFDM.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bin/libOFDM/CMakeFiles/libOFDM.dir/depend

