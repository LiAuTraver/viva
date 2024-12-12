cmake_minimum_required(VERSION 3.15)
project(viva C)

set(CMAKE_C_STANDARD 23)

set(VIVA_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE VIVA_HEADERS ${VIVA_SOURCE}/*.h)
include_directories(${VIVA_SOURCE})

add_executable(viva
        ${CMAKE_CURRENT_SOURCE_DIR}/test/main.c
        ${VIVA_HEADERS}
)
set_target_properties(viva PROPERTIES LINKER_LANGUAGE C)

add_executable(viva_test
        ${CMAKE_CURRENT_SOURCE_DIR}/test/forward_list_test.c
        ${VIVA_HEADERS}
)
set_target_properties(viva_test PROPERTIES LINKER_LANGUAGE C)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/projects/maze)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/projects/tree)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/projects/treev)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/projects/elevator)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/projects/topological)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/projects/stumgr)

add_executable(stack_test
        ${CMAKE_CURRENT_SOURCE_DIR}/test/stack.c
        ${VIVA_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/projects/maze/maze_stack.c
        ${CMAKE_CURRENT_SOURCE_DIR}/projects/maze/maze_stack.h
)
set_target_properties(stack_test PROPERTIES LINKER_LANGUAGE C)