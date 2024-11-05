cmake_minimum_required(VERSION 3.10)
project(treev LANGUAGES C VERSION 1.0)

if (NOT DEFINED CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 11)
endif ()
set(VIVA_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}../../include)
file(GLOB_RECURSE VIVA_HEADERS ${VIVA_SOURCE}/*.h)

add_executable(treev
        ${CMAKE_CURRENT_SOURCE_DIR}/treev.c
        ${VIVA_HEADERS}
)
set_target_properties(treev PROPERTIES LINKER_LANGUAGE C)