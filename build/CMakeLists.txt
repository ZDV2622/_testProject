#
# Copyright 2014-2015 Ettus Research LLC
# Copyright 2018 Ettus Research, a National Instruments Company
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

cmake_minimum_required(VERSION 3.5.1)
#project(INIT_USRP CXX)

set (PROJECT NR3GPP)

project (${PROJECT})






FIND_PATH(LIBCONFIG_INCLUDE_DIR libconfig.h
  /usr/local/include
  /usr/include
)

FIND_PATH(LIBCONFIGPP_INCLUDE_DIR libconfig.h++
  /usr/local/include
  /usr/include
)

FIND_LIBRARY(LIBCONFIG_LIBRARY config
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(LIBCONFIGPP_LIBRARY config++
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(LIBCONFIG_STATIC_LIBRARY "libconfig${CMAKE_STATIC_LIBRARY_SUFFIX}"
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(LIBCONFIGPP_STATIC_LIBRARY "libconfig++${CMAKE_STATIC_LIBRARY_SUFFIX}"
    /usr/local/lib
    /usr/lib
)


IF(LIBCONFIG_INCLUDE_DIR)
  IF(LIBCONFIG_LIBRARY)
    SET(LIBCONFIG_FOUND TRUE)
    SET(LIBCONFIG_LIBRARIES ${LIBCONFIG_LIBRARY})
    SET(LIBCONFIG_STATIC_LIBRARY_PATH ${LIBCONFIG_STATIC_LIBRARY})
  ENDIF(LIBCONFIG_LIBRARY)
ENDIF(LIBCONFIG_INCLUDE_DIR)

IF(LIBCONFIGPP_INCLUDE_DIR)
  IF(LIBCONFIGPP_LIBRARY)
    SET(LIBCONFIGPP_FOUND TRUE)
    SET(LIBCONFIGPP_LIBRARIES ${LIBCONFIGPP_LIBRARY})
    SET(LIBCONFIGPP_STATIC_LIBRARY_PATH ${LIBCONFIGPP_STATIC_LIBRARY})
  ENDIF(LIBCONFIGPP_LIBRARY)
ENDIF(LIBCONFIGPP_INCLUDE_DIR)

IF (LIBCONFIG_FOUND)
   IF (NOT LibConfig_FIND_QUIETLY)
      MESSAGE(STATUS "Found LibConfig++: ${LIBCONFIGPP_LIBRARIES}" )
      MESSAGE(STATUS "Found LibConfig: ${LIBCONFIG_LIBRARIES}")
      MESSAGE(STATUS "static LibConfig path: ${LIBCONFIG_STATIC_LIBRARY_PATH}")
   ENDIF (NOT LibConfig_FIND_QUIETLY)
ELSE (LIBCONFIG_FOUND)
   IF (LibConfig_FIND_REQUIRED)
      MESSAGE(SEND_ERROR "Could NOT find LibConfig")
   ENDIF (LibConfig_FIND_REQUIRED)
ENDIF (LIBCONFIG_FOUND)

MARK_AS_ADVANCED(LIBCONFIG_INCLUDE_DIR LIBCONFIG_LIBRARIES)




### Configure Compiler ########################################################
set(CMAKE_CXX_STANDARD 11)



if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_EXE_LINKER_FLAGS "-lthr ${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lconfig++ -lconfig")

### Set up build environment ##################################################
# Choose a static or shared-library build (shared is default, and static will
# probably need some special care!)
# Set this to ON in order to link a static build of UHD:
option(UHD_USE_STATIC_LIBS OFF)

# To add UHD as a dependency to this project, add a line such as this:
find_package(UHD 3.15 REQUIRED)

# The version in  ^^^^^  here is a minimum version.
# To specify an exact version:
#find_package(UHD 4.0.0 EXACT REQUIRED)

# This example also requires Boost.
# Set components here, then include UHDBoost to do the actual finding
set(UHD_BOOST_REQUIRED_COMPONENTS
    program_options
    system
    thread
)
set(BOOST_MIN_VERSION 1.58)
include(UHDBoost)

# need these include and link directories for the build
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${UHD_INCLUDE_DIRS}
)
link_directories(${Boost_LIBRARY_DIRS})

### Make the executable #######################################################
#add_executable(init_usrp init_usrp.cpp)

#add_executable (${PROJECT} ../main_run_radixN.cpp)

set(CMAKE_BUILD_TYPE "Release")
message(STATUS "******************************************************************************")
message(STATUS "* NOTE: When building your own app, you probably need all kinds of different  ")
message(STATUS "* compiler flags. This is just an example, so it's unlikely these settings    ")
message(STATUS "* exactly match what you require. Make sure to double-check compiler and     ")
message(STATUS "* linker flags to make sure your specific requirements are included.          ")
message(STATUS "******************************************************************************")


### Once it's built... ########################################################
# Here, you would have commands to install your program.
# We will skip these in this example.

#cmake_minimum_required (VERSION 3.5.1)

##set (PROJECT NR3GPP)

#project (${PROJECT})

include_directories (../)

#set(CMAKE_CXX_STANDARD 14)

set (LIBRARIES  
		libINIT
                libORAN
		libOFDM
                libNFAPI
                libTS38
    )

foreach (LIBRARY ${LIBRARIES})
	add_subdirectory (../${LIBRARY}/build bin/${LIBRARY})
endforeach ()

if (MSVC)
	add_definitions (/W4)
        ADD_DEFINITIONS(/arch:SSE)
elseif (CMAKE_COMPILER_IS_GNUCXX)
	add_definitions (-Wall -pedantic)
else ()
	message ("Unknown compiler")
endif ()

set (HEADERS)
set (SOURCES ../main_pbch_proc.cpp)

#source_group ("Header Files" FILES ${HEADERS})
#source_group ("Source Files" FILES ${SOURCES})

add_executable (${PROJECT} ${HEADERS} ${SOURCES})


target_link_libraries (${PROJECT} ${LIBRARIES} ${LIBCONFIG_LIBRARIES} ${LIBCONFIGPP_LIBRARIES})

# Shared library case: All we need to do is link against the library, and
# anything else we need (in this case, some Boost libraries):
if(NOT UHD_USE_STATIC_LIBS)
    message(STATUS "Linking against shared UHD library.")
    target_link_libraries(${PROJECT} ${UHD_LIBRARIES} ${Boost_LIBRARIES})
# Shared library case: All we need to do is link against the library, and
# anything else we need (in this case, some Boost libraries):
else(NOT UHD_USE_STATIC_LIBS)
    message(STATUS "Linking against static UHD library.")
    target_link_libraries(${PROJECT}
        # We could use ${UHD_LIBRARIES}, but linking requires some extra flags,
        # so we use this convenience variable provided to us
        ${UHD_STATIC_LIB_LINK_FLAG}
        # Also, when linking statically, we need to pull in all the deps for
        # UHD as well, because the dependencies don't get resolved automatically
        ${UHD_STATIC_LIB_DEPS}
    )
endif(NOT UHD_USE_STATIC_LIBS)


