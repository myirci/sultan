cmake_minimum_required(VERSION 2.8)
project(move_generator)

# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

add_definitions(-std=c++11)

aux_source_directory(./ SOURCES)
add_executable(${PROJECT_NAME} ${SOURCES})

