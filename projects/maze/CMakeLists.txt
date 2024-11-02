cmake_minimum_required(VERSION 3.15)
project(maze LANGUAGES C VERSION 1.0.0 DESCRIPTION "Maze")

if (NOT DEFINED CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 23)
endif ()

set(MAZE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE MAZE_HEADERS ${MAZE_SOURCE}/*.h)
include_directories(${MAZE_SOURCE})

set(VIVA_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}../../include)
file(GLOB_RECURSE VIVA_HEADERS ${VIVA_SOURCE}/*.h)
include_directories(${VIVA_SOURCE})

add_executable(maze
        ${CMAKE_CURRENT_SOURCE_DIR}/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/maze.c
        ${CMAKE_CURRENT_SOURCE_DIR}/maze_stack.c
        ${MAZE_HEADERS}
        ${VIVA_HEADERS}
        maze.h
)