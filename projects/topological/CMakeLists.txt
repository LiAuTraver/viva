cmake_minimum_required(VERSION 3.10)

project(topological VERSION 1.1.0 LANGUAGES C)

if (NOT DEFINED CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 11)
endif ()

set(VIVA_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}../../include)
file(GLOB_RECURSE VIVA_HEADERS ${VIVA_SOURCE}/*.h)
include_directories(${VIVA_SOURCE})

add_executable(topological
        topological.c
)


set_target_properties(topological PROPERTIES LINKER_LANGUAGE C)


