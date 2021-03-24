cmake_minimum_required (VERSION 3.5)

set (PROJECT libORAN)

project (${PROJECT})

set (HEADERS ../headers/ORAN_BS_SW_WG8.h
             )


set (SOURCES ../sources/ORAN_BS_SW_WG8.cpp
             )


#source_group ("Header Files" FILES ${HEADERS})
#source_group ("Source Files" FILES ${SOURCES})

add_library (${PROJECT} ${HEADERS} ${SOURCES})


