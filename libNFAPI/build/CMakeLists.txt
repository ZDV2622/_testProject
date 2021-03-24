cmake_minimum_required (VERSION 3.5)

set (PROJECT libNFAPI)

project (${PROJECT})

set (HEADERS ../headers/FAPI_SCF_222.h
             )


set (SOURCES ../sources/FAPI_SCF_222.cpp
             )


#source_group ("Header Files" FILES ${HEADERS})
#source_group ("Source Files" FILES ${SOURCES})

add_library (${PROJECT} ${HEADERS} ${SOURCES})
