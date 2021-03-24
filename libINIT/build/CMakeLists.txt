cmake_minimum_required (VERSION 3.5)

set (PROJECT libINIT)

project (${PROJECT})

set (HEADERS ../headers/_struct_zdv_cfg.h
             ../headers/_init.h
             )


set (SOURCES ../sources/_struct_zdv_cfg.cpp
             ../sources/_init.cpp
             )

#source_group ("Header Files" FILES ${HEADERS})
#source_group ("Source Files" FILES ${SOURCES})

add_library (${PROJECT} ${HEADERS} ${SOURCES})
