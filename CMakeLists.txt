cmake_minimum_required(VERSION 3.15)
project(viva C)
set(CMAKE_C_STANDARD 23)

set(VIVA_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# add all header files (for clion's code completion)
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

add_executable(tree
        ${CMAKE_CURRENT_SOURCE_DIR}/ projects/tree/tree.c
        ${VIVA_HEADERS}
)
set_target_properties(tree PROPERTIES LINKER_LANGUAGE C)

add_executable(tree_test
        ${CMAKE_CURRENT_SOURCE_DIR}/projects/tree/tree_test.c
        ${VIVA_HEADERS}
)
set_target_properties(tree_test PROPERTIES LINKER_LANGUAGE C)

add_executable(maze
        ${CMAKE_CURRENT_SOURCE_DIR}/projects/maze/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/projects/maze/maze.c
        ${VIVA_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/projects/maze/maze_stack.c
        ${CMAKE_CURRENT_SOURCE_DIR}/projects/maze/maze_stack.h
        projects/maze/maze.h
)
set_target_properties(maze PROPERTIES LINKER_LANGUAGE C)

add_executable(stack_test
        ${CMAKE_CURRENT_SOURCE_DIR}/test/stack.c
        ${VIVA_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/projects/maze/maze_stack.c
        ${CMAKE_CURRENT_SOURCE_DIR}/projects/maze/maze_stack.h
)
set_target_properties(stack_test PROPERTIES LINKER_LANGUAGE C)