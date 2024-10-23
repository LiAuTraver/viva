cmake_minimum_required(VERSION 3.30)
project(viva C)

set(CMAKE_C_STANDARD 23)

set(VIVA_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/include/net/ancillarycat/viva)
# add all header files
file(GLOB_RECURSE VIVA_HEADERS ${VIVA_SOURCE}/*.h)
include_directories(${VIVA_SOURCE})

add_executable(viva
        main.c
        ${VIVA_HEADERS}
)
add_executable(viva_test
        vtest.c
        ${VIVA_HEADERS}
)
add_executable(tree
        tree.c
        ${VIVA_HEADERS}
)
add_executable(tree_test
        tree_test.c
        ${VIVA_HEADERS}
)
