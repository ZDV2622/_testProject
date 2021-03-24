cmake_minimum_required (VERSION 3.5)

set (PROJECT libTemplate)

project (${PROJECT})

set (HEADERS ../headers/library_1.h)
set (SOURCES ../sources/library_1.cpp)

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

add_library (${PROJECT} ${HEADERS} ${SOURCES})
