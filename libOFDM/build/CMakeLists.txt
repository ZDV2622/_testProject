cmake_minimum_required (VERSION 3.5)

set (PROJECT libOFDM)

project (${PROJECT})

#set (HEADERS ../headers/*.h)
#set (SOURCES ../sources/*.cpp)

file(GLOB SOURCES
        "../sources/*.cpp"
        )

file(GLOB HEADERS
        "../headers/*.h"
        )

#source_group ("Header Files" FILES ${HEADERS})
#source_group ("Source Files" FILES ${SOURCES})

add_library (${PROJECT} ${HEADERS} ${SOURCES})
