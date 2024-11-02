cmake_minimum_required(VERSION 3.15)
project(tree LANGUAGES C VERSION 1.0.0 DESCRIPTION "Tree data structure")

if (NOT DEFINED CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 11)
endif ()
set(VIVA_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}../../include)
file(GLOB_RECURSE VIVA_HEADERS ${VIVA_SOURCE}/*.h)

add_executable(tree
        ${CMAKE_CURRENT_SOURCE_DIR}/tree.c
        ${VIVA_HEADERS}
)
set_target_properties(tree PROPERTIES LINKER_LANGUAGE C)

add_executable(tree_test
        ${CMAKE_CURRENT_SOURCE_DIR}/tree_test.c
        ${VIVA_HEADERS}
)
set_target_properties(tree_test PROPERTIES LINKER_LANGUAGE C)
